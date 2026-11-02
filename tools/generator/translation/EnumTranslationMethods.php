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
        $referenced = $this->findDeclaration(CPPName::type($type));
        if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
            return CPPName::type($type);
        }
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
        $constantGroups = [];
        foreach ($declaration['constants'] ?? [] as $constant) {
            $constantName = (string) ($constant['name'] ?? '');
            if ((!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))
                || !str_contains($constantName, '_')) {
                continue;
            }
            [$prefix, $case] = explode('_', $constantName, 2);
            $constantGroups[$prefix][$case] = (int) $constant['value'];
        }
        foreach ($constantGroups as $prefix => $cases) {
            if ($this->enumPrefixMatchesProperty($prefix, $normalizedName)) {
                return CPPName::enumCase($prefix);
            }
        }
        $matchingPrefixes = [];
        $semanticProperties = [];
        foreach ($declaration['properties'] ?? [] as $candidate) {
            if (($candidate['static'] ?? false)
                || ltrim((string) ($candidate['type'] ?? ''), '?') !== 'int') {
                continue;
            }
            $candidateName = strtolower((string) ($candidate['name'] ?? ''));
            foreach (array_keys($constantGroups) as $prefix) {
                if ($this->enumPrefixMatchesProperty($prefix, $candidateName)) {
                    $matchingPrefixes[$prefix] = true;
                }
            }
            if (preg_match('/(?:action|rotation|status|state|mode|type|result|request|event|property|reason|severity|algorithm)$/', $candidateName) === 1) {
                $semanticProperties[] = $candidateName;
            }
        }
        $numericConstants = array_sum(array_map(
            static fn (array $constant): int => is_int($constant['value'] ?? null) || is_bool($constant['value'] ?? null) ? 1 : 0,
            $declaration['constants'] ?? [],
        ));
        if (count($semanticProperties) === 1
            && $matchingPrefixes === []
            && $semanticProperties[0] === $normalizedName
            && $numericConstants >= 2) {
            return CPPName::enumCase((string) ($property['name'] ?? 'value'));
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
            'flag',
            'effect',
            'target',
        ];
        if (str_ends_with($propertyName, $prefix)) {
            return in_array($prefix, $semanticNames, true);
        }
        if (!str_starts_with($propertyName, $prefix)) {
            return false;
        }
        return in_array(substr($propertyName, strlen($prefix)), ['s', ...$semanticNames], true);
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
                $current = $this->findDeclaration($declaration['name']) ?? $declaration;
                $enumCase = $this->constantEnumCase($current, $name);
                if ($enumCase !== null) {
                    return explode('::', $enumCase, 2)[0];
                }
                return null;
            }
            $referenced = $this->findDeclaration($class);
            if ($referenced !== null) {
                $enumCase = $this->constantEnumCase($referenced, $name);
                if ($enumCase !== null) {
                    return explode('::', $enumCase, 2)[0];
                }
            }
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
        return 'magic_enum::enum_integer(' . $value . ')';
    }

private function enumReadValue(string $enumType, string $value, ?array $declaration = null): string
    {
        $default = $this->enumDefaultValue($enumType, $declaration);
        if ($default === null) {
            throw new \RuntimeException('Unable to resolve the first case of enum ' . $enumType);
        }
        if (str_starts_with($value, 'stream.readOptional(')) {
            $value = preg_replace_callback(
                '/return ([^\r\n]+);/',
                fn (array $match): string => 'return ' . $this->enumCastValue($enumType, $match[1], $default) . ';',
                $value,
            ) ?? $value;
            return $value;
        }
        return $this->enumCastValue($enumType, $value, $default);
    }

    private function enumCastValue(string $enumType, string $value, string $default): string
    {
        return 'magic_enum::enum_cast<' . $enumType . ">(\n"
            . "\t" . $value . "\n"
            . ').value_or(' . $default . ')';
    }

    private function enumDefaultValue(string $enumType, ?array $declaration): ?string
    {
        $name = ltrim($enumType, ':\\');
        $parts = explode('::', $name);
        $caseType = (string) end($parts);
        $outputType = $name;
        if ($caseType === 'BuildPlatform') {
            return 'BuildPlatform::Unknown';
        }
        $searchDeclaration = $declaration;
        if (count($parts) > 1) {
            $owner = implode('::', array_slice($parts, 0, -1));
            $searchDeclaration = $this->findDeclaration(CPPName::type($owner)) ?? $searchDeclaration;
        }
        $bitmaskValues = [];
        if ($searchDeclaration !== null) {
            foreach ($searchDeclaration['constants'] ?? [] as $constant) {
                $constantName = (string) ($constant['name'] ?? '');
                if ((!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))
                    || !str_contains($constantName, '_')) {
                    continue;
                }
                [$prefix] = explode('_', $constantName, 2);
                if (CPPName::enumCase($prefix) === $caseType) {
                    $bitmaskValues[] = (int) $constant['value'];
                }
            }
        }
        if (count($bitmaskValues) >= 2
            && !in_array(0, $bitmaskValues, true)
            && array_reduce(
                $bitmaskValues,
                static fn (bool $valid, int $value): bool => $valid && ($value > 0) && (($value & ($value - 1)) === 0),
                true,
            )) {
            return $outputType . '{}';
        }
        if ($searchDeclaration !== null) {
            foreach ($searchDeclaration['constants'] ?? [] as $constant) {
                $constantName = (string) ($constant['name'] ?? '');
                if (!is_int($constant['value'] ?? null)
                    && !is_bool($constant['value'] ?? null)) {
                    continue;
                }
                if (!str_contains($constantName, '_')) {
                    continue;
                }
                [$prefix, $case] = explode('_', $constantName, 2);
                if (CPPName::enumCase($prefix) === $caseType) {
                    return $outputType . '::' . CPPName::enumCase($case);
                }
            }
            foreach ($searchDeclaration['properties'] ?? [] as $property) {
                if ($this->propertyEnumType($property, $searchDeclaration) !== $caseType) {
                    continue;
                }
                $sourceType = ltrim((string) ($property['type'] ?? ''), '?');
                $propertyEnum = $this->findDeclaration(CPPName::type($sourceType));
                foreach ($propertyEnum['cases'] ?? [] as $case) {
                    if (is_string($case['name'] ?? null)) {
                        return $outputType . '::' . CPPName::enumCase($case['name']);
                    }
                }
                foreach ($searchDeclaration['constants'] ?? [] as $constant) {
                    $constantName = (string) ($constant['name'] ?? '');
                    if ((!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))
                        || !str_contains($constantName, '_')) {
                        continue;
                    }
                    [$prefix, $case] = explode('_', $constantName, 2);
                    if (CPPName::enumCase($prefix) === $caseType) {
                        return $outputType . '::' . CPPName::enumCase($case);
                    }
                }
            }
        }
        $referenced = $this->findDeclaration($caseType);
        foreach ($referenced['cases'] ?? [] as $case) {
            if (is_string($case['name'] ?? null)) {
                return $outputType . '::' . CPPName::enumCase($case['name']);
            }
        }
        foreach ($referenced['constants'] ?? [] as $constant) {
            if (is_int($constant['value'] ?? null) || is_bool($constant['value'] ?? null)) {
                return $outputType . '::' . CPPName::enumCase((string) $constant['name']);
            }
        }
        return $outputType . '{}';
    }

private function enumStorageCast(Node $argument, string $value, array $declaration): string
    {
        if (str_contains($value, 'magic_enum::enum_integer')) {
            return preg_replace(
                '/magic_enum::enum_integer\(magic_enum::enum_integer\(([^()]*)\)\)/',
                'magic_enum::enum_integer($1)',
                $value,
            ) ?? $value;
        }
        $propertyEnumType = $argument instanceof Node\Arg
            ? $this->enumPropertyType($argument->value, $declaration)
            : $this->enumPropertyType($argument, $declaration);
        $type = $propertyEnumType;
        if ($type === null && $argument instanceof Node\Arg) {
            $type = $this->expressionType($argument->value, $declaration);
        }
        if ($type === null && $argument instanceof Node\Arg && $argument->value instanceof Node\Expr\Variable) {
            $type = $this->expressionTypes[(string) $argument->value->name] ?? null;
        }
        $isEnum = $propertyEnumType !== null
            || ($type !== null && str_contains($type, '::') && !str_starts_with($type, 'std::') && !str_starts_with($type, 'glm::'))
            || ($type !== null && $this->isEnumLikeDeclaration($this->findDeclaration(CPPName::type($type)) ?? []))
            || ($type !== null && preg_match('/^[A-Z][A-Za-z0-9_]*$/', $type) === 1)
            || ($type !== null && $this->isInlineEnumType($type, $declaration));
        if (!$isEnum) {
            return $value;
        }
        return 'magic_enum::enum_integer(' . $value . ')';
    }

    private function isInlineEnumType(string $type, array $declaration): bool
    {
        if (in_array($type, ['Action', 'Type', 'Category', 'BuildPlatform'], true)) {
            return true;
        }
        foreach ($declaration['constants'] ?? [] as $constant) {
            $name = (string) ($constant['name'] ?? '');
            if (!str_contains($name, '_')
                || (!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))) {
                continue;
            }
            [$prefix] = explode('_', $name, 2);
            if (CPPName::enumCase($prefix) === $type) {
                return true;
            }
        }
        foreach ($declaration['properties'] ?? [] as $property) {
            $propertyEnumType = $this->propertyEnumType($property, $declaration);
            if ($propertyEnumType !== null
                && strcasecmp(ltrim($propertyEnumType, ':'), ltrim($type, ':')) === 0) {
                return true;
            }
        }
        return false;
    }
}
