<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait TypeEmissionMethods
{
    private function derivedTypes(array $declaration, array $types): array
    {
        $baseName = CPPName::type((string) ($declaration['name'] ?? ''));
        $derived = [];
        foreach ($types as $type) {
            if (CPPName::type((string) ($type['name'] ?? '')) === $baseName) {
                continue;
            }
            $parents = array_filter(
                array_merge(
                    [$type['parent'] ?? null],
                    is_array($type['implements'] ?? null) ? $type['implements'] : [],
                ),
                static fn (mixed $parent): bool => is_string($parent) && $parent !== '',
            );
            foreach ($parents as $parent) {
                if (CPPName::type($parent) === $baseName) {
                    $derived[] = $type;
                    break;
                }
            }
        }
        return $derived;
    }

    private function canonicalMethodName(string $name): string
    {
        return match (strtolower($name)) {
            'decode' => 'read',
            'encode' => 'write',
            default => $name,
        };
    }

    private function hasInheritedMethod(array $declaration, string $name, array $types): bool
    {
        $parents = array_filter(
            array_merge(
                [$declaration['parent'] ?? null],
                is_array($declaration['implements'] ?? null) ? $declaration['implements'] : [],
            ),
            static fn (mixed $parent): bool => is_string($parent) && $parent !== '',
        );
        foreach ($parents as $parent) {
            $parentName = CPPName::type($parent);
            foreach ($types as $type) {
                if (CPPName::type((string) ($type['name'] ?? '')) !== $parentName) {
                    continue;
                }
                if (($type['abstract'] ?? false) || ($type['kind'] ?? null) === 'interface') {
                    if (in_array($name, ['read', 'write'], true)) {
                        return true;
                    }
                }
                foreach ($type['methods'] ?? [] as $method) {
                    if (!($method['static'] ?? false)
                        && $this->canonicalMethodName((string) ($method['name'] ?? '')) === $name) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private function abstractMethodDeclaration(array $method, string $canonical, array $declaration): string
    {
        $returnType = $this->translator->cppType(
            (string) ($method['returnType'] ?? 'void'),
            false,
            $method['doc'] ?? null,
        );
        if ($canonical === 'read') {
            return '    virtual ' . $this->serializerMethodSignature($method, 'read', $this->hasContextSerializer($declaration, [])) . " = 0;\n";
        }
        if ($canonical === 'write') {
            return '    virtual ' . $this->serializerMethodSignature($method, 'write', $this->hasContextSerializer($declaration, [])) . " = 0;\n";
        }
        $parameters = [];
        foreach ($method['parameters'] ?? [] as $parameter) {
            $type = $this->translator->cppType(
                (string) ($parameter['type'] ?? 'mixed'),
                false,
                null,
            );
            $parameters[] = 'const ' . $type . '& ' . CPPName::identifier((string) ($parameter['name'] ?? 'value'));
        }
        return '    virtual ' . $returnType . ' ' . CPPName::identifier((string) ($method['name'] ?? 'method'))
            . '(' . implode(', ', $parameters) . ') = 0;' . "\n";
    }

    private function serializerParameters(array $method): string
    {
        $parameters = [];
        foreach ($method['parameters'] ?? [] as $parameter) {
            $sourceType = CPPName::type((string) ($parameter['type'] ?? ''));
            if (str_ends_with($sourceType, 'Reader') || str_contains($sourceType, 'ReadOnly')) {
                $parameters[] = 'ReadOnlyBinaryStream& stream';
                continue;
            }
            if (str_ends_with($sourceType, 'Writer') || str_contains($sourceType, 'BinaryStream')) {
                $parameters[] = 'BinaryStream& stream';
                continue;
            }
            $type = $this->translator->cppType(
                (string) ($parameter['type'] ?? 'mixed'),
                false,
                null,
            );
            $parameters[] = 'const ' . $type . '& ' . CPPName::identifier((string) ($parameter['name'] ?? 'value'));
        }
        return implode(', ', $parameters);
    }

    private function serializerMethodSignature(array $method, string $canonical, bool $includeContext): string
    {
        $streamType = $canonical === 'write' ? 'BinaryStream&' : 'ReadOnlyBinaryStream&';
        $parameters = [$streamType . ' stream'];
        $context = $includeContext ? array_slice($method['parameters'] ?? [], 1) : [];
        foreach ($context as $parameter) {
            $type = $this->translator->cppType(
                (string) ($parameter['type'] ?? 'mixed'),
                false,
                null,
            );
            $parameters[] = 'const ' . $type . '& ' . CPPName::identifier((string) ($parameter['name'] ?? 'value'));
        }
        return 'void ' . $canonical . '(' . implode(', ', $parameters) . ')' . ($canonical === 'write' ? ' const' : '');
    }

    private function hasContextSerializer(array $declaration, array $types): bool
    {
        if ($this->generatedTypeName($declaration) === 'GameRule') {
            return true;
        }
        $parentName = CPPName::type((string) ($declaration['parent'] ?? ''));
        if ($parentName === '') {
            return false;
        }
        foreach ($types as $type) {
            if ($this->generatedTypeName($type) === $parentName) {
                return $this->hasContextSerializer($type, $types);
            }
        }
        return false;
    }

    private function polymorphicFactory(string $baseName, array $derivedTypes): string
    {
        return "\n    static std::shared_ptr<" . $baseName . "> create(ReadOnlyBinaryStream& stream);\n";
    }

    private function polymorphicFactoryDefinition(string $baseName, array $derivedTypes): string
    {
        $cases = [];
        foreach ($derivedTypes as $derivedType) {
            if (($derivedType['abstract'] ?? false) || ($derivedType['kind'] ?? null) === 'interface') {
                continue;
            }
            $id = $this->constantInteger($derivedType, 'ID');
            if ($id === null) {
                continue;
            }
            $name = $this->generatedTypeName($derivedType);
            $cases[] = "        case " . $id . ": {\n"
                . '            auto value = std::make_shared<' . $name . ">();\n"
                . "            value->read(stream);\n"
                . "            return value;\n"
                . "        }\n";
        }
        if ($cases === []) {
            return '';
        }
        return "\ninline std::shared_ptr<" . $baseName . "> " . $baseName . "::create(ReadOnlyBinaryStream& stream) {\n"
            . "    switch (stream.readUnsignedVarInt().value_or(0)) {\n"
            . implode('', $cases)
            . "        default:\n            return nullptr;\n"
            . "    }\n}\n";
    }

    private function constructorDefinition(array $declaration, string $name, array $types): ?string
    {
        $constructor = null;
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['name'] ?? null) === '__construct') {
                $constructor = $method;
                break;
            }
        }
        if ($constructor === null || ($constructor['parameters'] ?? []) === []) {
            return null;
        }

        $properties = [];
        foreach ($declaration['properties'] ?? [] as $property) {
            $properties[(string) ($property['name'] ?? '')] = $property;
        }
        $parameters = [];
        $baseInitializers = [];
        $initializers = [];
        $parentDeclaration = null;
        $parentName = CPPName::type((string) ($declaration['parent'] ?? ''));
        foreach ($types as $candidate) {
            if ($this->generatedTypeName($candidate) === $parentName) {
                $parentDeclaration = $candidate;
                break;
            }
        }
        $parentParameterNames = [];
        foreach ($parentDeclaration['methods'] ?? [] as $parentMethod) {
            if (($parentMethod['name'] ?? null) !== '__construct') {
                continue;
            }
            foreach ($parentMethod['parameters'] ?? [] as $parentParameter) {
                $parentParameterNames[] = (string) ($parentParameter['name'] ?? '');
            }
            break;
        }
        $parentArguments = [];
        foreach ($constructor['parameters'] as $parameter) {
            $parameterName = (string) ($parameter['name'] ?? '');
            if ($parameterName === '') {
                return null;
            }
            $property = $properties[$parameterName] ?? null;
            if ($property === null) {
                foreach ($parentDeclaration['properties'] ?? [] as $candidateProperty) {
                    if (($candidateProperty['name'] ?? null) === $parameterName) {
                        $property = $candidateProperty;
                        break;
                    }
                }
            }
            if ($property === null) {
                return null;
            }
            $parameters[] = 'const ' . $this->propertyType($property, $declaration, false) . '& ' . CPPName::identifier($parameterName) . 'Value';
            if (!isset($properties[$parameterName]) && in_array($parameterName, $parentParameterNames, true)) {
                $parentArguments[$parameterName] = CPPName::identifier($parameterName . 'Value');
            } else {
                $initializers[] = CPPName::identifier($parameterName) . '(' . CPPName::identifier($parameterName) . 'Value)';
            }
        }
        if ($parentName !== '' && $parentArguments !== []) {
            $arguments = [];
            foreach ($parentParameterNames as $parameterName) {
                if (isset($parentArguments[$parameterName])) {
                    $arguments[] = $parentArguments[$parameterName];
                }
            }
            if ($arguments !== []) {
                $baseInitializers[] = $parentName . '(' . implode(', ', $arguments) . ')';
            }
        }
        $allInitializers = array_merge($baseInitializers, $initializers);
        $multiline = count($parameters) > 3 || strlen(implode(', ', $parameters)) > 100 || strlen(implode(', ', $allInitializers)) > 100;
        if ($multiline) {
            $body = '    explicit ' . $name . "(\n"
                . '        ' . implode(",\n        ", $parameters) . "\n"
                . '    )';
            if ($allInitializers !== []) {
                $body .= " :\n        " . implode(",\n        ", $allInitializers);
            }
        } else {
            $body = '    explicit ' . $name . '(' . implode(', ', $parameters) . ')'
                . ($allInitializers === [] ? '' : ' : ' . implode(', ', $allInitializers));
        }
        if (($constructor['body'] ?? []) === []) {
            return $body . " {}\n";
        }
        $aliases = [];
        foreach ($constructor['parameters'] as $parameter) {
            $parameterName = (string) ($parameter['name'] ?? '');
            $aliases[$parameterName] = CPPName::identifier($parameterName . 'Value');
        }
        $body .= " {\n" . $this->translator->methodBodyWithAliases($constructor, $declaration, $aliases) . "\n    }\n";
        return $body;
    }
}
