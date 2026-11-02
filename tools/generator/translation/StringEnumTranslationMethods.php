<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait StringEnumTranslationMethods
{
private function stringEnumName(array $property, array $declaration): ?string
    {
        if (strtolower(ltrim((string) ($property['type'] ?? ''), '?')) !== 'string') {
            return null;
        }
        return $this->stringEnumCases($property, $declaration) === []
            ? null
            : $this->stringEnumTypeName($property);
    }

private function stringEnumInfo(Node $node, array $declaration): ?array
    {
        if (!$node instanceof Node\Expr\PropertyFetch) {
            return null;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        if ($node->var instanceof Node\Expr\Variable && $node->var->name === 'this') {
            foreach ($declaration['properties'] ?? [] as $property) {
                $enum = $this->stringEnumName($property, $declaration);
                if ($enum === null && $this->stringEnumCases($property, $declaration) !== []) {
                    $enum = $this->stringEnumTypeName($property);
                }
                if (($property['name'] ?? null) === $name && $enum !== null) {
                    return [
                        'owner' => CPPName::type((string) $declaration['name']),
                        'property' => strtolower($name),
                        'enum' => $enum,
                        'first' => $this->stringEnumFirstCase($property, $declaration),
                    ];
                }
            }
            return null;
        }
        $objectType = $this->expressionType($node->var, $declaration);
        $referenced = $objectType === null ? null : $this->findDeclaration(CPPName::type($objectType));
        foreach ($referenced['properties'] ?? [] as $property) {
            $enum = $this->stringEnumName($property, $referenced);
            if ($enum === null && $this->stringEnumCases($property, $referenced) !== []) {
                $enum = $this->stringEnumTypeName($property);
            }
            if (($property['name'] ?? null) === $name && $enum !== null) {
                return [
                    'owner' => CPPName::type($objectType),
                    'property' => strtolower($name),
                    'enum' => $enum,
                    'first' => $this->stringEnumFirstCase($property, $referenced),
                ];
            }
        }
        return null;
    }

private function stringEnumFromString(Node $node, string $value, array $declaration): string
    {
        $info = $this->stringEnumInfo($node, $declaration);
        return $info === null
            ? $value
            : $this->stringEnumCast(
                $info['owner'] . '::' . $info['enum'],
                $value,
                $info['owner'] . '::' . $info['enum'] . '::' . $info['first'],
            );
    }

private function stringEnumToString(Node $node, string $value, array $declaration): string
    {
        $info = $this->stringEnumInfo($node, $declaration);
        return $info === null ? $value : 'magic_enum::enum_name(' . $value . ')';
    }

private function stringEnumFromStringForProperty(array $property, string $value, array $declaration): string
    {
        $enum = $this->stringEnumName($property, $declaration);
        $first = $this->stringEnumFirstCase($property, $declaration);
        return $this->stringEnumCast(
            $enum,
            $value,
            $enum . '::' . $first,
        );
    }

private function stringEnumCast(string $enumType, string $value, string $default): string
    {
        return 'magic_enum::enum_cast<' . $enumType . ">(\n"
            . "\tstd::string_view(" . $value . ")\n"
            . ').value_or(' . $default . ')';
    }

private function stringEnumToStringForProperty(array $property, string $value, array $declaration): string
    {
        return 'magic_enum::enum_name(' . $value . ')';
    }

private function stringEnumValues(array $property, array $declaration): array
    {
        return array_values($this->stringEnumCases($property, $declaration));
    }

private function stringEnumFirstCase(array $property, array $declaration): string
    {
        $cases = $this->stringEnumCases($property, $declaration);
        return $cases === [] ? 'Value' : CPPName::enumCase((string) array_key_first($cases));
    }

private function stringEnumTypeName(array $property): string
    {
        $name = (string) ($property['name'] ?? 'value');
        return str_contains($name, '_')
            ? CPPName::enumCase($name)
            : CPPName::identifier(ucfirst($name));
    }

    /** @return array<string, string> */
    private function stringEnumCases(array $property, array $declaration): array
    {
        if (strtolower(ltrim((string) ($property['type'] ?? ''), '?')) !== 'string') {
            return [];
        }
        $groups = [];
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (!is_string($constant['value'] ?? null)) {
                continue;
            }
            $constantName = (string) ($constant['name'] ?? '');
            if (!str_contains($constantName, '_')) {
                continue;
            }
            $parts = explode('_', $constantName);
            for ($index = 1; $index < count($parts); ++$index) {
                $prefix = implode('_', array_slice($parts, 0, $index));
                $case = implode('_', array_slice($parts, $index));
                $groups[$prefix][$case] = $constant['value'];
            }
        }
        if ($groups === []) {
            return [];
        }
        $propertyName = strtolower((string) ($property['name'] ?? ''));
        $bestPrefix = null;
        foreach (array_keys($groups) as $prefix) {
            if ($this->enumPrefixMatchesProperty($prefix, $propertyName)
                && ($bestPrefix === null || strlen($prefix) > strlen($bestPrefix))) {
                $bestPrefix = $prefix;
            }
        }
        if ($bestPrefix !== null) {
            return $groups[$bestPrefix];
        }
        if (preg_match('/^(?:type|operation|status|state|mode|value|flag|reason|category)$/i', $propertyName) === 1) {
            $best = null;
            foreach ($groups as $cases) {
                if ($best === null || count($cases) > count($best)) {
                    $best = $cases;
                }
            }
            return $best ?? [];
        }
        return [];
    }
}
