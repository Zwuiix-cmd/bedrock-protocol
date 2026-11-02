<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait EnumTranslationMethods
{
private function coerceVariantAlternative(Node\Expr $expression, string $value, ?string $expectedType, array $declaration): string
    {
        if ($expectedType === null || $expectedType === '') {
            return $value;
        }
        $sourceType = $this->expressionType($expression, $declaration);
        if ($sourceType === null) {
            return $value;
        }
        $actualType = $this->cppType($sourceType);
        $alternatives = $this->templateArguments($actualType, 'std::variant');
        if ($alternatives === [] || !in_array($expectedType, $alternatives, true)) {
            return $value;
        }
        return 'std::get<' . $expectedType . '>(' . $value . ')';
    }

private function enumPropertyType(Node $expression, array $declaration): ?string
    {
        if (!$expression instanceof Node\Expr\PropertyFetch) {
            return null;
        }
        $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
        if ($expression->var instanceof Node\Expr\Variable && $expression->var->name !== 'this') {
            $objectType = $this->expressionTypes[(string) $expression->var->name] ?? null;
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                foreach ($referenced['properties'] ?? [] as $property) {
                    if (($property['name'] ?? null) === $name) {
                        $enumType = $this->propertyEnumType($property, $referenced);
                        if (in_array($enumType, ['Type', 'Category'], true) || $this->stringEnumName($property, $referenced) !== null) {
                            return CPPName::type($objectType) . '::' . $enumType;
                        }
                        return $enumType;
                    }
                }
            }
            return null;
        }
        if (!$expression->var instanceof Node\Expr\Variable || $expression->var->name !== 'this') {
            return null;
        }
        foreach ($declaration['properties'] as $property) {
            if (($property['name'] ?? null) === $name) {
                return $this->propertyEnumType($property, $declaration);
            }
        }
        return null;
    }

private function propertyEnumType(array $property, array $declaration): ?string
    {
        $type = ltrim((string) ($property['type'] ?? ''), '?');
        if ($type !== 'int') {
            return $this->stringEnumName($property, $declaration);
        }
        $default = $property['defaultNode'] ?? null;
        if ($default instanceof Node\Expr\ClassConstFetch && $default->class instanceof Node\Name) {
            $class = $default->class->getLast();
            if ($this->catalog->constantAlias($class, $default->name instanceof Node\Identifier ? $default->name->toString() : '') !== null) {
                return 'BuildPlatform';
            }
        }
        $name = strtolower((string) ($property['name'] ?? ''));
        $normalizedName = preg_replace('/[^a-z0-9]/', '', $name) ?? $name;
        foreach ($this->declarations as $candidate) {
            if (!$this->isEnumLikeDeclaration($candidate)) {
                continue;
            }
            $candidateName = strtolower((string) ($candidate['name'] ?? ''));
            $normalizedCandidate = preg_replace('/[^a-z0-9]/', '', $candidateName) ?? $candidateName;
            if ($normalizedName === $normalizedCandidate) {
                $cppType = $this->catalog->typeAlias((string) $candidate['name']) ?? CPPName::type((string) $candidate['name']);
                return (string) ($property['name'] ?? '') === (string) ($candidate['name'] ?? '')
                    ? '::' . ltrim($cppType, ':')
                    : $cppType;
            }
        }
        $hasTypeConstants = false;
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (str_starts_with((string) ($constant['name'] ?? ''), 'TYPE_')) {
                $hasTypeConstants = true;
                break;
            }
        }
        return $hasTypeConstants && $name === 'type' ? 'Type' : null;
    }

private function stringEnumName(array $property, array $declaration): ?string
    {
        $propertyName = strtolower((string) ($property['name'] ?? ''));
        if ($propertyName === '') {
            return null;
        }
        $prefix = strtoupper($propertyName) . '_';
        $cases = [];
        foreach ($declaration['constants'] ?? [] as $constant) {
            $constantName = (string) ($constant['name'] ?? '');
            if (str_starts_with($constantName, $prefix) && is_string($constant['value'] ?? null)) {
                $cases[] = $constantName;
            }
        }
        return $cases === [] ? null : CPPName::enumCase($propertyName);
    }

private function stringEnumInfo(Node $node, array $declaration): ?array
    {
        if (!$node instanceof Node\Expr\PropertyFetch) {
            return null;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        if ($node->var instanceof Node\Expr\Variable && $node->var->name === 'this') {
            foreach ($declaration['properties'] ?? [] as $property) {
                if (($property['name'] ?? null) === $name && $this->stringEnumName($property, $declaration) !== null) {
                    return [
                        'owner' => CPPName::type((string) $declaration['name']),
                        'property' => strtolower($name),
                        'enum' => (string) $this->stringEnumName($property, $declaration),
                        'first' => CPPName::identifier((string) ($this->stringEnumValues($property, $declaration)[0] ?? '')),
                    ];
                }
            }
            return null;
        }
        $objectType = $this->expressionType($node->var, $declaration);
        $referenced = $objectType === null ? null : $this->findDeclaration(CPPName::type($objectType));
        foreach ($referenced['properties'] ?? [] as $property) {
            if (($property['name'] ?? null) === $name && $this->stringEnumName($property, $referenced) !== null) {
                return [
                    'owner' => CPPName::type($objectType),
                    'property' => strtolower($name),
                    'enum' => (string) $this->stringEnumName($property, $referenced),
                    'first' => CPPName::identifier((string) ($this->stringEnumValues($property, $referenced)[0] ?? '')),
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
            : 'magic_enum::enum_cast<' . $info['owner'] . '::' . $info['enum'] . '>(std::string_view(' . $value . ')).value_or(' . $info['owner'] . '::' . $info['enum'] . '::' . $info['first'] . ')';
    }

private function stringEnumToString(Node $node, string $value, array $declaration): string
    {
        $info = $this->stringEnumInfo($node, $declaration);
        return $info === null ? $value : 'magic_enum::enum_name(' . $value . ')';
    }

private function stringEnumFromStringForProperty(array $property, string $value, array $declaration): string
    {
        $enum = $this->stringEnumName($property, $declaration);
        $first = CPPName::identifier((string) ($this->stringEnumValues($property, $declaration)[0] ?? ''));
        return 'magic_enum::enum_cast<' . CPPName::type((string) $declaration['name']) . '::' . $enum . '>(std::string_view(' . $value . ')).value_or(' . CPPName::type((string) $declaration['name']) . '::' . $enum . '::' . $first . ')';
    }

private function stringEnumToStringForProperty(array $property, string $value, array $declaration): string
    {
        return 'magic_enum::enum_name(' . $value . ')';
    }

private function stringEnumValues(array $property, array $declaration): array
    {
        $prefix = strtoupper((string) ($property['name'] ?? '')) . '_';
        $values = [];
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (str_starts_with((string) ($constant['name'] ?? ''), $prefix) && is_string($constant['value'] ?? null)) {
                $values[] = (string) $constant['value'];
            }
        }
        return $values;
    }

private function classConstantType(Node $expression): ?string
    {
        if ($expression instanceof Node\Expr\Match_) {
            foreach ($expression->arms as $arm) {
                $type = $this->classConstantType($arm->body);
                if ($type !== null) {
                    return $type;
                }
            }
            return null;
        }
        if (!$expression instanceof Node\Expr\ClassConstFetch || !$expression->name instanceof Node\Identifier) {
            return null;
        }
        $name = $expression->name->toString();
        if (str_starts_with($name, 'TYPE_')) {
            return 'Type';
        }
        if (str_starts_with($name, 'CATEGORY_')) {
            return 'Category';
        }
        return null;
    }

private function enumExpressionType(Node $expression, array $declaration): ?string
    {
        if ($expression instanceof Node\Expr\ClassConstFetch) {
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            if ($class === 'self' || $class === 'static') {
                if (str_starts_with($name, 'TYPE_')) {
                    return 'Type';
                }
                if (str_starts_with($name, 'CATEGORY_')) {
                    return 'Category';
                }
                return null;
            }
            $referenced = $this->findDeclaration($class);
            return $referenced !== null && $this->isEnumLikeDeclaration($referenced)
                ? CPPName::type($class)
                : null;
        }
        if ($expression instanceof Node\Expr\PropertyFetch) {
            $type = $this->expressionType($expression, $declaration);
            if ($type !== null && $this->isEnumLikeDeclaration($this->findDeclaration(CPPName::type($type)) ?? [])) {
                return CPPName::type($type);
            }
        }
        return $this->enumPropertyType($expression, $declaration);
    }

private function enumStorageValue(string $value, string $enumType): string
    {
        return 'static_cast<std::underlying_type_t<decltype(' . $value . ')>>(' . $value . ')';
    }

private function enumStorageCast(Node $argument, string $value, array $declaration): string
    {
        $type = $argument instanceof Node\Arg
            ? $this->enumPropertyType($argument->value, $declaration)
            : $this->enumPropertyType($argument, $declaration);
        if ($type === null && $argument instanceof Node\Arg) {
            $type = $this->expressionType($argument->value, $declaration);
        }
        if ($type === null && $argument instanceof Node\Expr\Variable) {
            $type = $this->expressionTypes[(string) $argument->name] ?? null;
        }
        $isEnum = in_array($type, ['Type', 'Category', 'BuildPlatform'], true)
            || ($type !== null && str_contains($type, '::') && !str_starts_with($type, 'std::') && !str_starts_with($type, 'glm::'))
            || ($type !== null && $this->isEnumLikeDeclaration($this->findDeclaration(CPPName::type($type)) ?? []));
        if (!$isEnum) {
            return $value;
        }
        return 'static_cast<int32_t>(' . $value . ')';
    }
}
