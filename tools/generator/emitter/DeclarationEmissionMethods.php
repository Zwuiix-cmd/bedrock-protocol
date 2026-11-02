<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait DeclarationEmissionMethods
{
    private function writeForwardDeclarations(string $output, array $types): void
    {
        $body = "#pragma once\n\n#include <cstdint>\n\n";
        foreach ($types as $type) {
            if ($this->isEnumDeclaration($type)) {
                $body .= 'enum class ' . $this->generatedTypeName($type) . ' : ' . $this->enumUnderlyingType($type) . ";\n";
            } else {
                $body .= 'class ' . $this->generatedTypeName($type) . ";\n";
            }
        }
        $this->write($output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'Forward.hpp', $body);
    }

    private function writePacketIds(string $output, array $packets): void
    {
        $names = array_map(static fn (array $packet): string => CPPName::packetId($packet['name']), $packets);
        $width = max(7, ...array_map('strlen', $names));
        $body = "#pragma once\n\n#include <cstdint>\n\nenum class PacketIds : int {\n";
        $body .= str_pad('    Unknown', $width + 4) . " = 0,\n";
        foreach ($packets as $packet) {
            $name = CPPName::packetId($packet['name']);
            $body .= str_pad('    ' . $name, $width + 4) . ' = ' . (int) $packet['networkId'] . ",\n";
        }
        $body .= "};\n";
        $path = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . 'PacketIds.hpp';
        $this->write($path, $body);
    }

    private function writeEnum(string $output, array $declaration): void
    {
        $name = $this->generatedTypeName($declaration);
        $body = "#pragma once\n\n#include <cstdint>\n#include <magic_enum/magic_enum.hpp>\n#include <string_view>\n\n";
        $cases = $this->enumCases($declaration);
        $enumInclude = $this->generatedTypeInclude($declaration);
        $body .= 'enum class ' . $name . ' : ' . $this->enumUnderlyingType($declaration) . " {\n";
        $caseLines = [];
        foreach ($cases as $index => $case) {
            $value = $case['value'] ?? $index;
            $value = is_int($value) || is_float($value) ? (string) $value : (string) $index;
            $caseLines[] = '    ' . CPPName::enumCase((string) $case['name']) . ' = ' . $value . ",\n";
        }
        if (count($caseLines) > 300) {
            $directory = dirname($enumInclude);
            $basename = pathinfo($enumInclude, PATHINFO_FILENAME);
            foreach (array_chunk($caseLines, 200) as $chunkIndex => $chunk) {
                $fragment = $basename . 'Cases' . $chunkIndex . '.inc';
                $fragmentInclude = ($directory === '.' ? '' : $directory . '/') . $fragment;
                $fragmentPath = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock'
                    . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR
                    . str_replace('/', DIRECTORY_SEPARATOR, $fragmentInclude);
                $this->write($fragmentPath, implode('', $chunk));
                $body .= '#include <bedrock/protocol/' . $fragmentInclude . ">\n";
            }
        } else {
            $body .= implode('', $caseLines);
        }
        $body .= "};\n";
        if ($this->isStringEnumDeclaration($declaration)) {
            $nameCaseLines = [];
            foreach ($cases as $case) {
                if (!is_string($case['value'] ?? null)) {
                    continue;
                }
                $nameCaseLines[] = '        case ' . $name . '::' . CPPName::enumCase((string) $case['name'])
                    . ': return ' . json_encode($case['value'], JSON_UNESCAPED_SLASHES | JSON_THROW_ON_ERROR) . ";\n";
            }
            $body .= "namespace magic_enum::customize {\n\n";
            $body .= 'template <> constexpr customize_t enum_name<' . $name . '>('
                . $name . " value) noexcept {\n"
                . "    switch (value) {\n";
            if (count($nameCaseLines) > 300) {
                $directory = dirname($enumInclude);
                $basename = pathinfo($enumInclude, PATHINFO_FILENAME);
                foreach (array_chunk($nameCaseLines, 200) as $chunkIndex => $chunk) {
                    $fragment = $basename . 'Names' . $chunkIndex . '.inc';
                    $fragmentInclude = ($directory === '.' ? '' : $directory . '/') . $fragment;
                    $fragmentPath = $output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock'
                        . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR
                        . str_replace('/', DIRECTORY_SEPARATOR, $fragmentInclude);
                    $this->write($fragmentPath, implode('', $chunk));
                    $body .= '#include <bedrock/protocol/' . $fragmentInclude . ">\n";
                }
            } else {
                $body .= implode('', $nameCaseLines);
            }
            $body .= "        default: return invalid_tag;\n"
                . "    }\n}\n\n}\n";
        }
        $this->write($this->typePath($output, $name, $declaration), $this->uniqueIncludes($body));
    }

    private function uniqueIncludes(string $body): string
    {
        $seen = [];
        $lines = explode("\n", $body);
        $result = [];
        foreach ($lines as $line) {
            if (preg_match('/^#include\s+<([^>]+)>$/', $line, $matches) === 1) {
                if (isset($seen[$matches[1]])) {
                    continue;
                }
                $seen[$matches[1]] = true;
            }
            $result[] = $line;
        }
        return implode("\n", $result);
    }

    private function writeType(string $output, array $declaration, array $types): void
    {
        $name = $this->generatedTypeName($declaration);
        $body = "#pragma once\n\n__GENERATED_STANDARD_INCLUDES__\n";
        $propertyTypes = [];
        foreach ($declaration['properties'] as $property) {
            if (!($property['static'] ?? false)) {
                $propertyTypes[] = $this->propertyType($property, $declaration, false);
            }
        }
        if (array_filter($propertyTypes, static fn (string $type): bool => str_contains($type, 'std::variant') || str_contains($type, 'std::monostate')) !== []) {
            $body .= "#include <variant>\n";
        }
        if (array_filter($propertyTypes, static fn (string $type): bool => str_contains($type, 'std::map<')) !== []) {
            $body .= "#include <map>\n";
        }
        if (array_filter($propertyTypes, static fn (string $type): bool => str_contains($type, 'std::unordered_map<')) !== []) {
            $body .= "#include <unordered_map>\n";
        }
        if ($name === 'GameRule') {
            $body .= "#include <map>\n";
        }
        foreach ($this->runtimeIncludes($declaration['properties'], $declaration) as $include) {
            $body .= '#include <bedrock/protocol/' . $include . ">\n";
        }
        $referencedNames = [];
        foreach ($declaration['properties'] as $property) {
            $emittedType = $this->propertyType($property, $declaration, false);
            if (!str_contains($emittedType, '<') && !in_array($emittedType, [
                'bool', 'char', 'int8_t', 'uint8_t', 'int16_t', 'uint16_t',
                'int32_t', 'uint32_t', 'int64_t', 'uint64_t', 'float', 'double',
                'std::string', 'std::string_view', 'glm::vec2', 'glm::vec3', 'glm::ivec3',
            ], true)) {
                $referencedNames[] = CPPName::type($emittedType);
            }
            foreach (array_filter([
                (string) ($property['type'] ?? ''),
                $this->propertyDocType((string) ($property['doc'] ?? '')),
            ]) as $sourceType) {
                foreach (explode('|', $sourceType) as $part) {
                    $part = trim(ltrim($part, '?'));
                    if (preg_match('/^(?:array|list)\s*<\s*(?:[^,>]+\s*,\s*)?(.+)>$/is', $part, $matches) === 1) {
                        $part = trim($matches[1]);
                    }
                    if ($part !== '') {
                        $referencedNames[] = CPPName::type($part);
                    }
                }
            }
        }
        foreach ($types as $type) {
            $typeName = $this->generatedTypeName($type);
            if ($typeName === $name) {
                continue;
            }
            foreach ($propertyTypes as $propertyType) {
                if (preg_match(
                    '/(?<![A-Za-z0-9_])' . preg_quote($typeName, '/') . '(?![A-Za-z0-9_])/',
                    $propertyType,
                ) === 1) {
                    $referencedNames[] = $typeName;
                    break;
                }
            }
        }
        $referencedNames = array_unique($referencedNames);
        foreach ($types as $type) {
            $typeName = $this->generatedTypeName($type);
            if ($typeName === $name || !in_array($typeName, $referencedNames, true)) {
                continue;
            }
            $body .= '#include <bedrock/protocol/' . $this->generatedTypeInclude($type) . ">\n";
        }
        $isAbstractType = ($declaration['abstract'] ?? false) || ($declaration['kind'] ?? null) === 'interface';
        if (!$isAbstractType) {
            foreach ($this->methodReferencedTypes($declaration, $types) as $type) {
                if ($this->generatedTypeName($type) !== $name) {
                    $body .= '#include <bedrock/protocol/' . $this->generatedTypeInclude($type) . ">\n";
                }
            }
        }
        if (!$isAbstractType) {
            foreach ($this->methodReferencedIncludes($declaration) as $include) {
                $body .= '#include <bedrock/protocol/' . $include . ">\n";
            }
        }
        $parentName = null;
        $parent = $declaration['parent'] ?? null;
        if (is_string($parent) && $parent !== '') {
            $candidate = CPPName::type($parent);
            $parentIsGenerated = false;
            $parentDeclaration = null;
            foreach ($types as $type) {
                if ($this->generatedTypeName($type) === $candidate && !$this->isEnumDeclaration($type)) {
                    $parentIsGenerated = true;
                    $parentDeclaration = $type;
                    break;
                }
            }
            if ($parentIsGenerated && $parentDeclaration !== null) {
                $body .= '#include <bedrock/protocol/' . $this->generatedTypeInclude($parentDeclaration) . ">\n";
                $parentName = $candidate;
            }
        }
        $bases = [];
        if ($parentName !== null) {
            $bases[] = $parentName;
        }
        foreach ($declaration['implements'] ?? [] as $implemented) {
            $candidate = CPPName::type((string) $implemented);
            foreach ($types as $type) {
                if ($this->generatedTypeName($type) === $candidate && !$this->isEnumDeclaration($type)) {
                    $body .= '#include <bedrock/protocol/' . $this->generatedTypeInclude($type) . ">\n";
                    $bases[] = $candidate;
                    break;
                }
            }
        }
        $inheritance = $bases === [] ? '' : ' : public ' . implode(', public ', array_unique($bases));
        $isAbstractType = ($declaration['abstract'] ?? false) || ($declaration['kind'] ?? null) === 'interface';
        $derivedTypes = $isAbstractType ? $this->derivedTypes($declaration, $types) : [];
        $body .= "\nclass " . $name . $inheritance . " {\npublic:\n";
        $body .= $this->constants($declaration);
        foreach ($declaration['properties'] as $property) {
            if ($property['static']) {
                continue;
            }
            $type = $this->propertyType($property, $declaration, false);
            $mutable = $this->propertyAssignedBySerializer($declaration, (string) $property['name'], 'write') ? 'mutable ' : '';
            $body .= '    ' . $mutable . $type . ' ' . CPPName::identifier($property['name']);
            $default = $this->propertyInitializer($property, $declaration, $type, $types);
            $body .= $default === null ? ";\n" : ' = ' . $default . ";\n";
        }
        $body .= "\n    " . $name . "() = default;\n";
        if ($isAbstractType || $bases !== []) {
            $body .= "    virtual ~" . $name . "() = default;\n";
        }
        $constructor = $this->constructorDefinition($declaration, $name, $types);
        if ($constructor !== null) {
            $body .= $constructor;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            if (!($method['static'] ?? false)) {
                continue;
            }
            $canonical = $this->canonicalMethodName((string) ($method['name'] ?? ''));
            if (!in_array($canonical, ['read', 'write'], true)
                || !in_array(strtolower((string) ($method['returnType'] ?? '')), ['self', 'static'], true)
                || count($method['parameters'] ?? []) <= 1) {
                continue;
            }
            $staticName = 'create';
            $body .= "\n    static std::shared_ptr<" . $name . "> " . $staticName
                . '(' . $this->serializerParameters($method) . ") {\n";
            $body .= $this->indentGeneratedBody(
                $this->translator->methodBody($method, $declaration, true),
                0,
            ) . "\n";
            $body .= "    }\n";
        }
        $hasAbstractRead = false;
        $hasAbstractWrite = false;
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['name'] ?? null) !== 'getTypeId' || ($method['static'] ?? false)) {
                continue;
            }
            $returnType = $this->translator->cppType(
                (string) ($method['returnType'] ?? 'int32_t'),
                false,
                $method['doc'] ?? null,
            );
            $hasId = false;
            foreach ($declaration['constants'] ?? [] as $constant) {
                if (($constant['name'] ?? null) === 'ID') {
                    $hasId = true;
                    break;
                }
            }
            if ($method['abstract'] ?? false) {
                $body .= "\n    virtual " . $returnType . " getTypeId() const = 0;\n";
            } else {
                $override = $this->hasInheritedMethod($declaration, 'getTypeId', $types) ? ' override' : '';
                $body .= "\n    " . ($override === '' ? 'virtual ' : '') . $returnType . " getTypeId() const" . $override . " { return " . ($hasId ? 'static_cast<' . $returnType . '>(ID)' : '{}') . "; }\n";
            }
            break;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['static'] ?? false) || !($method['abstract'] ?? false)) {
                continue;
            }
            $canonical = $this->canonicalMethodName((string) ($method['name'] ?? ''));
            if ($canonical === 'getTypeId' || !in_array($canonical, ['read', 'write'], true)) {
                continue;
            }
            if ($canonical === 'write') {
                $hasAbstractWrite = true;
            } elseif ($canonical === 'read') {
                $hasAbstractRead = true;
            }
            $body .= $this->abstractMethodDeclaration($method, $canonical, $declaration);
        }
        if ($isAbstractType && !$hasAbstractRead) {
            $body .= "    virtual void read(ReadOnlyBinaryStream& stream) = 0;\n";
        }
        if ($isAbstractType && !$hasAbstractWrite) {
            $body .= "    virtual void write(BinaryStream& stream) const = 0;\n";
        }
        if ($isAbstractType && $derivedTypes !== []) {
            $body .= $this->polymorphicFactory($name, $derivedTypes);
        }
        if ($name === 'GameRule') {
            $body .= "\n    static std::shared_ptr<GameRule> create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame);\n";
            $body .= "    static std::map<std::string, std::shared_ptr<GameRule>> readRules(ReadOnlyBinaryStream& stream, const bool& isStartGame);\n";
            $body .= "    static void writeRules(BinaryStream& stream, const std::map<std::string, std::shared_ptr<GameRule>>& rules, const bool& isStartGame);\n";
        }
        if (!$isAbstractType) {
            $read = $this->method($declaration, 'read');
            $readAbstract = $read !== null && ($read['abstract'] ?? false);
            $supportsContext = $this->hasContextSerializer($declaration, $types);
            $readSignature = $read !== null && !($read['static'] ?? false) && $supportsContext
                ? $this->serializerMethodSignature($read, 'read', true)
                : 'void read(ReadOnlyBinaryStream& stream)';
            $body .= "\n    " . ($this->hasInheritedMethod($declaration, 'read', $types) ? $readSignature . ' override' : 'virtual ' . $readSignature) . " {\n";
            $body .= ($read === null || ($read['static'] ?? false) || $readAbstract || (!$supportsContext && count($read['parameters'] ?? []) > 1)
                ? $this->translator->genericBody($declaration, true)
                : $this->translator->methodBody($read, $declaration)) . "\n";
            $body .= "    }\n\n";
            $write = $this->method($declaration, 'write');
            $writeAbstract = $write !== null && ($write['abstract'] ?? false);
            $writeSignature = $write !== null && !($write['static'] ?? false) && $supportsContext
                ? $this->serializerMethodSignature($write, 'write', true)
                : 'void write(BinaryStream& stream) const';
            $body .= "    " . ($this->hasInheritedMethod($declaration, 'write', $types) ? $writeSignature . ' override' : 'virtual ' . $writeSignature) . " {\n";
            $body .= ($write === null || ($write['static'] ?? false) || $writeAbstract || (!$supportsContext && count($write['parameters'] ?? []) > 1)
                ? $this->translator->genericBody($declaration, false)
                : $this->translator->methodBody($write, $declaration)) . "\n";
            $body .= "    }\n";
        }
        $body .= "};\n";
        $body .= $this->stringEnumCustomizations($declaration);
        $body = str_replace(
            '__GENERATED_STANDARD_INCLUDES__',
            $this->standardTypeIncludes($body),
            $body,
        );
        $this->write($this->typePath($output, $name, $declaration), $this->uniqueIncludes($body));
    }

    private function standardTypeIncludes(string $body): string
    {
        $includes = [
            '<bedrock/protocol/io/BinaryStream.hpp>',
            '<bedrock/protocol/io/ReadOnlyBinaryStream.hpp>',
        ];
        $patterns = [
            '<magic_enum/magic_enum.hpp>' => 'magic_enum::',
            '<array>' => 'std::array<',
            '<cstddef>' => 'std::(?:size_t|ptrdiff_t)',
            '<cstdint>' => '\\b(?:u?int(?:8|16|32|64)_t|uint8_t|uint16_t|uint32_t|uint64_t)\\b|enum class [A-Za-z_]\\w*\\s*:',
            '<exception>' => 'std::exception',
            '<limits>' => 'std::numeric_limits<',
            '<memory>' => 'std::shared_ptr<|std::make_shared<',
            '<optional>' => 'std::optional<|std::nullopt',
            '<stdexcept>' => 'std::runtime_error|std::logic_error|std::invalid_argument',
            '<string>' => 'std::string(?:[>;)]|\\s)',
            '<string_view>' => 'std::string_view',
            '<tuple>' => 'std::tuple<',
            '<type_traits>' => 'std::(?:is_|remove_|underlying_type)',
            '<unordered_map>' => 'std::unordered_map<',
            '<utility>' => 'std::(?:move|forward|swap|pair)',
            '<variant>' => 'std::(?:variant|monostate|get<|visit\\()',
            '<vector>' => 'std::vector<',
            '<map>' => 'std::map<',
        ];
        foreach ($patterns as $include => $pattern) {
            if (preg_match('/' . $pattern . '/', $body) === 1) {
                $includes[] = $include;
            }
        }
        return implode("\n", array_map(
            static fn (string $include): string => '#include ' . $include,
            $includes,
        )) . "\n";
    }

    private function indentGeneratedBody(string $body, int $indent): string
    {
        $prefix = str_repeat("\t", $indent);
        return implode("\n", array_map(
            static fn (string $line): string => $line === '' ? '' : $prefix . $line,
            explode("\n", $body),
        ));
    }
}
