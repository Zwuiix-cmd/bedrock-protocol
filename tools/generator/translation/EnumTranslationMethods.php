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
                $objectType = preg_replace('/^std::shared_ptr<(.+)>$/', '$1', $objectType) ?? $objectType;
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
        if ($type !== '' && $this->catalog->typeCodec($type) !== null) {
            return null;
        }
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
        $matchingGroups = [];
        foreach ($constantGroups as $prefix => $cases) {
            if (count($cases) >= 2 && $this->enumPrefixMatchesProperty($prefix, $normalizedName)) {
                $matchingGroups[$prefix] = $cases;
            }
        }
        if ($matchingGroups !== []) {
            uksort(
                $matchingGroups,
                static fn (string $left, string $right): int => strlen($right) <=> strlen($left),
            );
            $prefix = (string) array_key_first($matchingGroups);
            $cases = $matchingGroups[$prefix];
            $nestedPrefixes = [];
            foreach (array_keys($cases) as $case) {
                if (str_contains((string) $case, '_')) {
                    [$nested] = explode('_', (string) $case, 2);
                    $nestedPrefixes[$nested] = true;
                }
            }
            if (count($nestedPrefixes) === 1
                && count(array_filter(
                    array_keys($cases),
                    static fn (string $case): bool => str_contains($case, '_'),
                )) === count($cases)) {
                return CPPName::enumCase((string) array_key_first($nestedPrefixes));
            }
            return CPPName::enumCase($prefix);
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
        $multiCaseGroups = array_filter(
            $constantGroups,
            static fn (array $cases): bool => count($cases) >= 2,
        );
        if (count($semanticProperties) === 1
            && $matchingPrefixes === []
            && $semanticProperties[0] === $normalizedName
            && $numericConstants >= 2
            && count($multiCaseGroups) !== 1) {
            return CPPName::enumCase((string) ($property['name'] ?? 'value'));
        }
        $hasTypeConstants = false;
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (str_starts_with((string) ($constant['name'] ?? ''), 'TYPE_')) {
                $hasTypeConstants = true;
                break;
            }
        }
        if ($hasTypeConstants && $name === 'type') {
            return 'Type';
        }
        if ($name === 'type') {
            $candidateGroups = array_filter(
                $constantGroups,
                static fn (array $cases): bool => count($cases) >= 2,
            );
            if (count($candidateGroups) === 1) {
                return CPPName::enumCase((string) array_key_first($candidateGroups));
            }
        }
        return null;
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
                $alias = $this->catalog->typeAlias($class);
                if ($alias !== null && $this->isEnumLikeDeclaration($this->findDeclaration($alias) ?? [])) {
                    return CPPName::type($alias);
                }
                if ($alias === null && $this->isEnumLikeDeclaration($referenced)) {
                    return CPPName::type($class);
                }
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
        if ($this->isStringEnumType($enumType)) {
            return $value;
        }
        return str_contains($value, 'magic_enum::enum_integer')
            ? $value
            : 'magic_enum::enum_integer(' . $value . ')';
    }

    private function isStringEnumType(string $enumType): bool
    {
        $parts = explode('::', ltrim($enumType, ':'));
        $declaration = $this->findDeclaration((string) end($parts));
        if ($declaration === null) {
            return false;
        }
        if (($declaration['backingType'] ?? null) === 'string') {
            return true;
        }
        if (($declaration['kind'] ?? null) !== 'class' || ($declaration['properties'] ?? []) !== []) {
            return false;
        }
        $constants = array_filter(
            $declaration['constants'] ?? [],
            static fn (array $constant): bool => ($constant['name'] ?? null) !== 'NETWORK_ID',
        );
        return $constants !== []
            && array_reduce(
                $constants,
                static fn (bool $valid, array $constant): bool => $valid && is_string($constant['value'] ?? null),
                true,
            );
    }

    private function enumComparableValue(string $value, string $enumType): string
    {
        if (preg_match('/^magic_enum::enum_integer\((.+)\)$/', trim($value), $matches) !== 1) {
            return $value;
        }
        $inner = trim($matches[1]);
        $normalizedType = ltrim($enumType, ':');
        if (str_starts_with($inner, $normalizedType . '::')) {
            return $inner;
        }
        $parts = explode('::', $inner);
        if (count($parts) >= 2 && CPPName::type($parts[count($parts) - 2]) === CPPName::type($enumType)) {
            return $inner;
        }
        return $value;
    }

    private function enumStorageClassConstant(
        Node\Expr\ClassConstFetch $fetch,
        string $value,
        array $declaration,
    ): string {
        if (str_contains($value, 'magic_enum::enum_integer')) {
            return $value;
        }
        $class = $fetch->class instanceof Node\Name ? $fetch->class->getLast() : '';
        $name = $fetch->name instanceof Node\Identifier ? $fetch->name->toString() : '';
        $owner = $class === 'self' || $class === 'static'
            ? ($this->findDeclaration($declaration['name']) ?? $declaration)
            : $this->findDeclaration($class);
        $ownerData = $owner ?? [];
        $stringEnum = ($ownerData['backingType'] ?? null) === 'string';
        if (!$stringEnum && ($ownerData['kind'] ?? null) === 'class' && ($ownerData['properties'] ?? []) === []) {
            $constants = array_filter(
                $ownerData['constants'] ?? [],
                static fn (array $constant): bool => ($constant['name'] ?? null) !== 'NETWORK_ID',
            );
            $stringEnum = $constants !== []
                && array_reduce(
                    $constants,
                    static fn (bool $valid, array $constant): bool => $valid && is_string($constant['value'] ?? null),
                    true,
                );
        }
        if ($stringEnum) {
            return 'magic_enum::enum_name(' . $value . ')';
        }
        if ($owner !== null && $this->isEnumLikeDeclaration($owner)) {
            return 'magic_enum::enum_integer(' . $value . ')';
        }
        foreach ($owner['constants'] ?? [] as $constant) {
            if (($constant['name'] ?? null) !== $name
                || (!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))) {
                continue;
            }
            if (str_contains($name, '_') && $this->constantEnumCase($owner, $name) !== null) {
                return 'magic_enum::enum_integer(' . $value . ')';
            }
        }
        return $value;
    }

private function enumReadValue(string $enumType, string $value, ?array $declaration = null): string
    {
        if ($this->isBitmaskEnum($enumType, $declaration)) {
            return 'static_cast<' . $enumType . '>(' . $value . ')';
        }
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

    private function isBitmaskEnum(string $enumType, ?array $declaration): bool
    {
        $name = ltrim($enumType, ':\\');
        $parts = explode('::', $name);
        $caseType = (string) end($parts);
        $values = [];

        if ($declaration !== null) {
            foreach ($declaration['constants'] ?? [] as $constant) {
                $constantName = (string) ($constant['name'] ?? '');
                if ((!is_int($constant['value'] ?? null) && !is_bool($constant['value'] ?? null))
                    || !str_contains($constantName, '_')) {
                    continue;
                }
                [$prefix] = explode('_', $constantName, 2);
                if (CPPName::enumCase($prefix) === $caseType) {
                    $values[] = (int) $constant['value'];
                }
            }
        }

        if ($values === []) {
            $referenced = $this->findDeclaration($caseType);
            foreach ($referenced['cases'] ?? [] as $case) {
                if (is_int($case['value'] ?? null) || is_bool($case['value'] ?? null)) {
                    $values[] = (int) $case['value'];
                }
            }
        }

        return count($values) >= 2
            && !in_array(0, $values, true)
            && array_reduce(
                $values,
                static fn (bool $valid, int $value): bool => $valid && $value > 0 && (($value & ($value - 1)) === 0),
                true,
            );
    }

private function enumStorageCast(Node $argument, string $value, array $declaration): string
    {
        $argumentType = $argument instanceof Node\Arg
            ? $this->assignmentCPPType($argument->value, $declaration)
            : null;
        if (($argument instanceof Node\Arg && $this->isOptionalProperty($argument->value, $declaration))
            || ($argumentType !== null && str_starts_with($argumentType, 'std::optional<'))) {
            return preg_replace(
                '/^magic_enum::enum_integer\((.*)\)$/s',
                '$1',
                $value,
            ) ?? $value;
        }
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
