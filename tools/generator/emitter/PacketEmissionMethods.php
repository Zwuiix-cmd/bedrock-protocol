<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait PacketEmissionMethods
{
    private function writePacket(string $output, array $packet, array $types): void
    {
        $name = CPPName::type($packet['name']);
        $generatedIncludes = [];
        foreach ($types as $type) {
            $generatedIncludes[$this->generatedTypeName($type)] = $this->generatedTypeInclude($type);
            $generatedIncludes[CPPName::type((string) ($type['name'] ?? ''))] = $this->generatedTypeInclude($type);
        }
        $body = "#pragma once\n\n#include <cstdint>\n#include <exception>\n#include <limits>\n#include <memory>\n#include <optional>\n#include <stdexcept>\n#include <string>\n#include <string_view>\n#include <tuple>\n#include <type_traits>\n#include <variant>\n#include <vector>\n#include <bedrock/protocol/Packet.hpp>\n";
        $included = [];
        foreach ($packet['properties'] as $property) {
            $propertyTypes = [CPPName::type((string) ($property['type'] ?? ''))];
            $docType = $this->propertyDocType((string) ($property['doc'] ?? ''));
            if ($docType !== null) {
                $propertyTypes[] = CPPName::type($docType);
                if (preg_match('/^(?:array|list)\s*<\s*(?:[^,>]+\s*,\s*)?(.+)>$/is', $docType, $elementMatches) === 1) {
                    $propertyTypes[] = CPPName::type(trim($elementMatches[1]));
                } elseif (str_ends_with($docType, '[]')) {
                    $propertyTypes[] = CPPName::type(substr($docType, 0, -2));
                }
            }
            foreach (array_unique($propertyTypes) as $typeName) {
                if (isset($generatedIncludes[$typeName]) && !isset($included[$typeName])) {
                    $body .= '#include <bedrock/protocol/' . $generatedIncludes[$typeName] . ">\n";
                    $included[$typeName] = true;
                }
            }
        }
        foreach ($types as $type) {
            if (!$this->isEnumDeclaration($type)) {
                continue;
            }
            $typeName = $this->generatedTypeName($type);
            if (!isset($included[$typeName])) {
                $body .= '#include <bedrock/protocol/' . $this->generatedTypeInclude($type) . ">\n";
                $included[$typeName] = true;
            }
        }
        foreach ($this->runtimeIncludes($packet['properties'], $packet) as $include) {
            $body .= '#include <bedrock/protocol/' . $include . ">\n";
        }
        $body .= "#include <bedrock/protocol/types/TypesGenerated.hpp>\n";
        $body .= "\nclass " . $name . " : public Packet {\npublic:\n";
        $body .= '    static constexpr PacketIds ID = PacketIds::' . CPPName::packetId($packet['name']) . ";\n\n";
        $body .= "    PacketIds getId() override {\n        return ID;\n    }\n\npublic:\n";
        $body .= $this->constants($packet);
        foreach ($packet['properties'] as $property) {
            if ($property['static']) {
                continue;
            }
            $type = $this->propertyType($property, $packet, true);
            $body .= '    ' . $type . ' ' . CPPName::identifier($property['name']);
            $default = $this->propertyInitializer($property, $packet, $type);
            $body .= $default === null ? ";\n" : ' = ' . $default . ";\n";
        }
        $body .= "\npublic:\n    std::string_view getName() override {\n        return magic_enum::enum_name(ID);\n    }\n\n";
        $decode = $this->method($packet, 'decodePayload');
        $encode = $this->method($packet, 'encodePayload');
        $body .= "    void read(ReadOnlyBinaryStream& stream) override {\n";
        $body .= $decode === null
            ? $this->translator->genericBody($packet, true)
            : $this->translator->methodBody($decode, $packet) . "\n";
        $body .= "    }\n\n    void write(BinaryStream& stream) override {\n";
        $body .= $encode === null
            ? $this->translator->genericBody($packet, false)
            : $this->translator->methodBody($encode, $packet) . "\n";
        $body .= "    }\n};\n";
        $this->write($output . DIRECTORY_SEPARATOR . 'include' . DIRECTORY_SEPARATOR . 'bedrock' . DIRECTORY_SEPARATOR . 'protocol' . DIRECTORY_SEPARATOR . $name . '.hpp', $body);
    }

    private function propertyType(array $property, array $declaration, bool $packet): string
    {
        return $this->translator->cppPropertyType($property, $declaration);
    }

    private function runtimeIncludes(array $properties, ?array $declaration = null): array
    {
        $includes = [];
        foreach ($properties as $property) {
            $types = array_filter([
                (string) ($property['type'] ?? ''),
                $this->propertyDocType((string) ($property['doc'] ?? '')),
            ]);
            if ($declaration !== null) {
                $types[] = $this->propertyType($property, $declaration, false);
            }
            foreach (array_filter($types) as $type) {
                $runtimeInclude = $this->includeForType($type);
                if ($runtimeInclude !== null) {
                    $includes[$runtimeInclude] = true;
                }
                foreach ($this->catalog->includesFor($type) as $include) {
                    $includes[$this->resolveTypeInclude($include)] = true;
                }
            }
            $defaultNode = $property['defaultNode'] ?? null;
            if ($defaultNode instanceof \PhpParser\Node\Expr\ClassConstFetch
                && $defaultNode->class instanceof \PhpParser\Node\Name
                && $this->catalog->constantAlias(
                    $defaultNode->class->getLast(),
                    $defaultNode->name instanceof \PhpParser\Node\Identifier ? $defaultNode->name->toString() : '',
                ) !== null) {
                $includes[$this->typeIncludePaths['BuildPlatform'] ?? 'types/BuildPlatform.hpp'] = true;
            }
            $text = strtolower(implode(' ', [
                (string) ($property['name'] ?? ''),
                (string) ($property['type'] ?? ''),
                (string) ($property['doc'] ?? ''),
            ]));
            if (str_contains($text, 'metadata') || str_contains($text, 'compoundtag')) {
                $includes[$this->typeIncludePaths['MetadataProperty'] ?? 'types/MetadataProperty.hpp'] = true;
            }
            if (str_contains($text, 'cacheablenbt') || str_contains($text, 'nbt')) {
                $includes['types/NetworkNbt.hpp'] = true;
            }
        }
        ksort($includes);
        return array_keys($includes);
    }

    private function propertyDocType(string $doc): ?string
    {
        if (preg_match('/@(?:var|phpstan-var)\s+([^\r\n]+)/', $doc, $matches) !== 1) {
            return null;
        }
        $type = preg_replace('/\s*\*\/?\s*$/', '', trim($matches[1])) ?? trim($matches[1]);
        foreach (explode('|', trim($type)) as $part) {
            $part = trim($part);
            if (strtolower($part) !== 'null' && $part !== '') {
                return $part;
            }
        }
        return null;
    }

    private function constants(array $declaration): string
    {
        $integer = [];
        $strings = [];
        $computed = [];
        $enumGroups = [];
        $stringEnums = [];
        foreach ($declaration['constants'] as $constant) {
            if ($constant['name'] === 'NETWORK_ID') {
                continue;
            }
            if (is_int($constant['value']) || is_bool($constant['value'])) {
                $name = (string) $constant['name'];
                if (str_contains($name, '_')) {
                    [$prefix, $case] = explode('_', $name, 2);
                    $enumGroups[$prefix][$case] = (int) $constant['value'];
                    continue;
                }
                $integer[$name] = (int) $constant['value'];
            } elseif (is_string($constant['value'])) {
                $strings[$constant['name']] = $constant['value'];
            } elseif (($constant['value'] ?? null) === null && ($constant['valueNode'] ?? null) instanceof \PhpParser\Node) {
                $value = $this->translator->expression($constant['valueNode'], $declaration);
                if (!str_contains($value, 'std::monostate')) {
                    $computed[(string) $constant['name']] = $value;
                }
            }
        }
        foreach ($strings as $name => $value) {
            if (preg_match('/^([A-Z][A-Z0-9]*)_(.+)$/', (string) $name, $matches) !== 1) {
                continue;
            }
            $prefix = strtolower((string) $matches[1]);
            foreach ($declaration['properties'] ?? [] as $property) {
                $propertyName = strtolower((string) ($property['name'] ?? ''));
                if ($propertyName !== $prefix) {
                    continue;
                }
                $stringEnums[$prefix][(string) $matches[2]] = $value;
                unset($strings[$name]);
                break;
            }
        }
        $body = '';
        foreach ($stringEnums as $prefix => $cases) {
            $enumName = CPPName::enumCase($prefix);
            $body .= '    enum class ' . $enumName . " : uint8_t {\n";
            foreach (array_values($cases) as $index => $case) {
                $body .= '        ' . CPPName::identifier((string) $case) . ' = ' . $index . ",\n";
            }
            $body .= "    };\n\n";
        }

        $matchingPrefixes = [];
        $semanticProperties = [];
        foreach ($declaration['properties'] ?? [] as $property) {
            if (($property['static'] ?? false)
                || !in_array(strtolower(ltrim((string) ($property['type'] ?? ''), '?')), ['int', 'integer', 'uint8', 'uint16', 'uint32', 'uint64'], true)) {
                continue;
            }
            $propertyName = strtolower((string) ($property['name'] ?? ''));
            foreach (array_keys($enumGroups) as $prefix) {
                if ($this->enumPrefixMatchesProperty($prefix, $propertyName)) {
                    $matchingPrefixes[$prefix] = true;
                }
            }
            if (preg_match('/(?:action|rotation|status|state|mode|type|result|request|event|property|reason|severity|algorithm)$/', $propertyName) === 1) {
                $semanticProperties[] = $propertyName;
            }
        }

        $constantCount = count($integer) + array_sum(array_map('count', $enumGroups));
        if ($matchingPrefixes === [] && count($semanticProperties) === 1 && $constantCount >= 2) {
            $semanticProperty = $semanticProperties[0];
            $combined = [];
            foreach ($enumGroups as $prefix => $cases) {
                foreach ($cases as $case => $value) {
                    $combinedCase = $case;
                    if (isset($combined[$combinedCase])) {
                        $combinedCase = $prefix . '_' . $case;
                    }
                    $combined[$combinedCase] = $value;
                }
            }
            foreach ($integer as $name => $value) {
                $combined[$name] = $value;
            }
            $enumGroups = [CPPName::enumCase($semanticProperty) => $combined];
            $integer = [];
        }
        foreach ($enumGroups as $prefix => $cases) {
            $emit = count($cases) >= 2
                || isset($matchingPrefixes[$prefix])
                || in_array(strtoupper((string) $prefix), ['TYPE', 'CATEGORY'], true);
            if (!$emit) {
                foreach ($cases as $case => $value) {
                    $integer[$prefix . '_' . $case] = $value;
                }
                continue;
            }
            $minimum = min($cases);
            $maximum = max($cases);
            $underlying = $minimum >= 0 && $maximum <= 0xff ? 'uint8_t' : 'int64_t';
            $body .= '    enum class ' . CPPName::enumCase($prefix) . ' : ' . $underlying . " {\n";
            foreach ($cases as $name => $value) {
                $body .= '        ' . CPPName::enumCase((string) $name) . ' = ' . $value . ",\n";
            }
            $body .= "    };\n\n";
        }
        if ($integer !== []) {
            foreach ($integer as $name => $value) {
                $body .= '    static constexpr int64_t ' . CPPName::identifier($name) . ' = ' . $value . ";\n";
            }
            $body .= "\n";
        }
        foreach ($computed as $name => $value) {
            $body .= '    static constexpr auto ' . CPPName::identifier($name) . ' = ' . $value . ";\n";
        }
        if ($computed !== []) {
            $body .= "\n";
        }
        foreach ($strings as $name => $value) {
            $body .= '    static constexpr std::string_view ' . CPPName::identifier($name) . ' = "' . addcslashes($value, "\\\"") . "\";\n";
        }
        return $body;
    }

    private function enumPrefixMatchesProperty(string $prefix, string $propertyName): bool
    {
        $prefix = preg_replace('/[^a-z0-9]/', '', strtolower($prefix)) ?? strtolower($prefix);
        $propertyName = preg_replace('/[^a-z0-9]/', '', strtolower($propertyName)) ?? strtolower($propertyName);
        if ($prefix === '' || $propertyName === $prefix) {
            return $prefix !== '';
        }
        $semanticNames = [
            'type',
            'action',
            'id',
            'mode',
            'state',
            'status',
            'result',
            'request',
            'event',
            'property',
            'reason',
            'severity',
            'algorithm',
            'operation',
            'value',
            'kind',
            'level',
            'order',
            'layout',
            'visibility',
            'category',
            'rotation',
            'layer',
            'layerid',
            'scheme',
            'permission',
            'permissions',
            'effect',
            'target',
        ];
        if (str_ends_with($propertyName, $prefix)) {
            return in_array($prefix, $semanticNames, true);
        }
        if (!str_starts_with($propertyName, $prefix)) {
            return false;
        }
        $suffix = substr($propertyName, strlen($prefix));
        return in_array($suffix, [
            's',
            ...$semanticNames,
        ], true);
    }
}
