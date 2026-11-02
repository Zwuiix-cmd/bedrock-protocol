<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait BodyTranslationMethods
{
public function methodBody(array $method, array $declaration, bool $returnShared = false): string
    {
        $statements = $method['body'] ?? [];
        if ($statements === []) {
            return $this->genericBody($declaration, $method['static'] || str_contains((string) $method['name'], 'decode'), $returnShared);
        }
        $previousLocalVariables = $this->localVariables;
        $previousDeclaredVariables = $this->declaredVariables;
        $this->localVariables = array_fill_keys($this->nestedAssignmentVariables($statements), true);
        $this->declaredVariables = [];
        foreach ($statements as $statement) {
            $this->inferLocalVariableTypes($statement);
        }
        $this->arrayScopes[] = $this->inferArrayScope($statements, $declaration);
        $lines = [];
        foreach ($this->nestedAssignmentNodes($statements) as $name => $assignment) {
            $value = $this->expression($assignment->expr, $declaration, $method);
            $type = $this->inferredCPPType($assignment->expr, $declaration) ?? 'std::remove_cvref_t<decltype(' . $value . ')>';
            $lines[] = "\t\t" . $type . ' ' . $this->variableName($name, $declaration) . "{};";
            $this->declaredVariables[$name] = true;
        }
        foreach ($statements as $statement) {
            $translated = $this->statement($statement, $declaration, $method, 2, $returnShared);
            if ($translated !== '') {
                $lines[] = $translated;
            }
        }
        array_pop($this->arrayScopes);
        if ($lines === []) {
            $this->localVariables = $previousLocalVariables;
            $this->declaredVariables = $previousDeclaredVariables;
            return $this->genericBody($declaration, $method['static'] || str_contains((string) $method['name'], 'decode'), $returnShared);
        }
        $body = implode("\n", $lines);
        if (str_contains($body, 'std::monostate')) {
            $this->localVariables = $previousLocalVariables;
            $this->declaredVariables = $previousDeclaredVariables;
            return $this->genericBody($declaration, $method['static'] || str_contains((string) $method['name'], 'decode'), $returnShared);
        }
        $this->localVariables = $previousLocalVariables;
        $this->declaredVariables = $previousDeclaredVariables;
        return $body;
    }

private function inferLocalVariableTypes(mixed $node): void
    {
        if (!$node instanceof Node) {
            return;
        }
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\Variable
            && $node->var->name !== 'this') {
            if ($node->expr instanceof Node\Expr\New_ && $node->expr->class instanceof Node\Name) {
                $this->expressionTypes[(string) $node->var->name] = $node->expr->class->getLast();
            } else {
                $type = $this->inferredCPPType($node->expr, ['properties' => [], 'constants' => []]);
                if ($type !== null) {
                    $this->expressionTypes[(string) $node->var->name] = $type;
                }
            }
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->{$name};
            if ($child instanceof Node) {
                $this->inferLocalVariableTypes($child);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->inferLocalVariableTypes($item);
                    }
                }
            }
        }
    }

public function methodBodyWithAliases(array $method, array $declaration, array $parameterAliases): string
    {
        $previousAliases = $this->expressionAliases;
        $previousTypes = $this->expressionTypes;
        foreach ($parameterAliases as $name => $alias) {
            $this->expressionAliases[$name] = $alias;
        }
        foreach ($method['parameters'] ?? [] as $parameter) {
            $name = (string) ($parameter['name'] ?? '');
            $type = (string) ($parameter['type'] ?? '');
            if ($name !== '' && $type !== '') {
                $this->expressionTypes[$name] = $type;
            }
        }
        $unsupportedBefore = count($this->unsupported);
        $body = $this->methodBody($method, $declaration);
        if (count($this->unsupported) !== $unsupportedBefore) {
            $body = '';
        }
        $this->expressionAliases = $previousAliases;
        $this->expressionTypes = $previousTypes;
        return $body;
    }

public function genericBody(array $declaration, bool $read, bool $returnShared = false): string
    {
        $hasSerializerMethod = false;
        foreach ($declaration['methods'] ?? [] as $method) {
            if (in_array(strtolower((string) ($method['name'] ?? '')), ['read', 'decode', 'write', 'encode'], true)) {
                $hasSerializerMethod = true;
                break;
            }
        }
        if ($hasSerializerMethod) {
            $codecBody = $this->sourceCodecBody($declaration, $read, $returnShared);
            if ($codecBody !== null) {
                return $codecBody;
            }
        }

        $name = CPPName::type((string) $declaration['name']);

        if ($read && $returnShared) {
            $lines = ['        auto result = std::make_shared<' . $name . '>();'];
        } else {
            $lines = [];
        }

        foreach ($this->orderedProperties($declaration) as $property) {
            if ($property['static']) {
                continue;
            }
            $field = CPPName::identifier((string) $property['name']);
            $target = $read && $returnShared ? 'result->' . $field : $field;
            if ($read) {
                $objectType = $this->objectPropertyType($property, $declaration);
                if ($objectType !== null && !$objectType['pointer']) {
                    $lines[] = '        ' . $target . '.read(stream);';
                    continue;
                }
                if ($objectType !== null) {
                    $lines[] = '        ' . $target . ' = std::make_shared<' . $objectType['type'] . '>();';
                    $lines[] = '        ' . $target . '->read(stream);';
                    continue;
                }
                $inferredValue = $this->inferredPropertyReadValue($declaration, $property);
                $value = $inferredValue ?? $this->fallbackReadValue(
                    $property['type'] ?? null,
                    $property['doc'] ?? null,
                    $declaration,
                );
                $enumType = $this->propertyEnumType($property, $declaration);
                if ($enumType === null) {
                    $propertyCPPType = $this->cppPropertyType($property, $declaration);
                    if ($this->isInlineEnumType($propertyCPPType, $declaration)) {
                        $enumType = $propertyCPPType;
                    }
                }
                if ($enumType !== null && $this->catalog->typeCodec($enumType) !== null) {
                    $enumType = null;
                }
                if ($enumType !== null) {
                    $stringEnum = $this->stringEnumName($property, $declaration);
                    if ($stringEnum !== null) {
                        $rawValue = $field . 'Raw';
                        $lines[] = '        const auto ' . $rawValue . ' = ' . $value . ';';
                        $value = $this->stringEnumFromStringForProperty(
                            $property,
                            $rawValue,
                            $declaration,
                        );
                    } else {
                        if (!str_contains($value, 'magic_enum::enum_cast')) {
                            $value = $this->enumReadValue($enumType, $value, $declaration);
                        }
                    }
                }
                $lines[] = '        ' . $target . ' = '
                    . $this->indentExpressionContinuation($value, '        ') . ';';
            } elseif (!$returnShared) {
                $enumType = $this->propertyEnumType($property, $declaration);
                if ($enumType === null) {
                    $propertyCPPType = $this->cppPropertyType($property, $declaration);
                    if ($this->isInlineEnumType($propertyCPPType, $declaration)) {
                        $enumType = $propertyCPPType;
                    }
                }
                if ($enumType !== null && $this->catalog->typeCodec($enumType) !== null) {
                    $enumType = null;
                }
                $isStringEnum = $this->stringEnumName($property, $declaration) !== null
                    || $this->stringEnumCases($property, $declaration) !== [];
                $inferredValue = $isStringEnum
                    ? 'magic_enum::enum_name(' . $field . ')'
                    : $field;
                $inferredWrite = $this->inferredPropertyWriteValue($declaration, $property, $inferredValue);
                if ($inferredWrite !== null && $enumType === null) {
                    $inferredWrite = preg_replace(
                        '/magic_enum::enum_integer\(magic_enum::enum_integer\(([^()]*)\)\)/',
                        'magic_enum::enum_integer($1)',
                        $inferredWrite,
                    ) ?? $inferredWrite;
                    $lines[] = $this->indentGeneratedStatement($inferredWrite, '        ');
                    continue;
                }
                $value = $field;
                if ($enumType !== null) {
                    $isNullable = $this->nullableType((string) ($property['type'] ?? ''))
                        || $this->nullableType($this->docType($property['doc'] ?? null) ?? '');
                    if ($isNullable) {
                        $baseType = $this->withoutNullType((string) ($property['type'] ?? ''));
                        $optionalWrite = $this->fallbackWriteValue(
                            $baseType,
                            null,
                            'optionalValue',
                            $declaration,
                        );
                        $optionalWriteStatement = 'stream.writeOptional('
                            . $field
                            . ', '
                            . $this->callback('const auto& optionalValue', $optionalWrite)
                            . ');';
                        $lines[] = $this->indentGeneratedStatement($optionalWriteStatement, '        ');
                        continue;
                    }
                    $value = $isStringEnum
                        ? $this->stringEnumToStringForProperty($property, $value, $declaration)
                        : (str_contains($value, 'magic_enum::enum_integer')
                            ? $value
                            : 'magic_enum::enum_integer(' . $value . ')');
                }
                $write = $this->fallbackWriteValue(
                    $property['type'] ?? null,
                    $property['doc'] ?? null,
                    $value,
                    $declaration,
                );
                $lines[] = $this->indentGeneratedStatement($write, '        ');
            }
        }

        if ($read && $returnShared) {
            $lines[] = '        return result;';
        }
        if ($lines === []) {
            return '        (void)stream;';
        }
        return implode("\n", $lines);
    }

    private function indentExpressionContinuation(string $value, string $prefix): string
    {
        $lines = explode("\n", $value);
        $first = array_shift($lines) ?? '';
        foreach ($lines as &$line) {
            $line = $prefix . str_replace("\t", '    ', $line);
        }
        unset($line);
        return implode("\n", [$first, ...$lines]);
    }

    private function indentGeneratedStatement(string $value, string $prefix): string
    {
        $lines = explode("\n", $value);
        foreach ($lines as &$line) {
            $line = $prefix . str_replace("\t", '    ', $line);
        }
        unset($line);
        return implode("\n", $lines);
    }

    /** @return array{type: string, pointer: bool}|null */
    private function objectPropertyType(
        array $property,
        array $declaration,
        bool $allowOptional = false,
    ): ?array
    {
        $rawType = (string) ($property['type'] ?? '');
        $sourceType = ltrim($rawType, '?');
        if ($rawType === ''
            || (!$allowOptional && str_starts_with($rawType, '?'))
            || str_contains($sourceType, '|')
            || preg_match('/^(?:array|list)\s*</i', $sourceType) === 1
            || str_ends_with($sourceType, '[]')) {
            return null;
        }
        $short = CPPName::type($sourceType);
        $referenced = $this->findDeclaration($short);
        if ($referenced !== null
            && (($referenced['kind'] ?? null) === 'enum'
            || ($referenced['abstract'] ?? false)
            || ($referenced['kind'] ?? null) === 'interface'
            || $this->isEnumLikeDeclaration($referenced))) {
            return null;
        }
        $cppType = $this->cppPropertyType($property, $declaration);
        if ($allowOptional && preg_match('/^std::optional<(.+)>$/', $cppType, $optionalType) === 1) {
            $cppType = $optionalType[1];
        }
        $codec = $this->catalog->typeCodec($short);
        if (is_array($codec)
            && is_string($codec['read'] ?? null)
            && preg_match('/\b[A-Za-z_]\w*\s+value;\s*value\.read\(stream\)/s', $codec['read']) !== 1) {
            return null;
        }
        if ($cppType === ''
            || str_starts_with($cppType, 'std::optional<')
            || str_starts_with($cppType, 'std::variant<')
            || str_starts_with($cppType, 'glm::')
            || in_array($cppType, [
                'bool',
                'char',
                'int8_t',
                'uint8_t',
                'int16_t',
                'uint16_t',
                'int32_t',
                'uint32_t',
                'int64_t',
                'uint64_t',
                'float',
                'double',
                'std::string',
                'std::string_view',
            ], true)) {
            return null;
        }
        if (preg_match('/^std::shared_ptr<([A-Za-z_]\w*)>$/', $cppType, $matches) === 1) {
            return ['type' => $matches[1], 'pointer' => true];
        }
        if (!str_contains($cppType, '<')
            && (($referenced !== null && $cppType === $short)
                || $this->catalog->typeAlias($short) === $cppType)) {
            return ['type' => $cppType, 'pointer' => false];
        }
        return null;
    }

    private function isDirectObjectProperty(array $property, array $declaration): bool
    {
        $objectType = $this->objectPropertyType($property, $declaration);
        return $objectType !== null && !$objectType['pointer'];
    }

    private function directReadAssignment(Node\Expr\Assign $assignment, array $declaration, array $method): ?string
    {
        if ($assignment->var instanceof Node\Expr\ArrayDimFetch) {
            return null;
        }
        if ($assignment->expr instanceof Node\Expr\MethodCall
            && $assignment->expr->var instanceof Node\Expr\New_
            && $assignment->expr->var->class instanceof Node\Name
            && $assignment->expr->name instanceof Node\Identifier
            && $this->catalog->staticCallAlias(
                $assignment->expr->var->class->getLast(),
                $assignment->expr->name->toString(),
            ) !== null) {
            return null;
        }
        if (!$this->isDeserializerExpression($assignment->expr)
            && !($assignment->expr instanceof Node\Expr\StaticCall)
            && !($assignment->expr instanceof Node\Expr\MethodCall)) {
            return null;
        }

        $objectType = null;
        if ($assignment->var instanceof Node\Expr\PropertyFetch) {
            if ($assignment->var->var instanceof Node\Expr\Variable
                && $assignment->var->var->name === 'this'
                && $assignment->var->name instanceof Node\Identifier) {
                $propertyName = $assignment->var->name->toString();
                foreach ($declaration['properties'] ?? [] as $property) {
                    if (($property['name'] ?? null) === $propertyName) {
                        $objectType = $this->objectPropertyType($property, $declaration, true);
                        break;
                    }
                }
            }
            if ($objectType === null) {
                $objectType = $this->objectTypeFromCppType(
                    $this->assignmentCPPType($assignment->var, $declaration),
                );
            }
        } elseif ($assignment->var instanceof Node\Expr\Variable) {
            $objectType = $this->objectTypeFromCppType(
                $this->inferredCPPType($assignment->expr, $declaration),
            );
        }
        if ($objectType === null) {
            $translated = $this->expression($assignment->expr, $declaration, $method);
            if (preg_match('/std::make_shared<([A-Za-z_]\w*)>\(\)/', $translated, $pointerType) === 1
                && str_contains($translated, '->read(stream)')) {
                $objectType = ['type' => $pointerType[1], 'pointer' => true];
            } elseif (preg_match('/\b([A-Za-z_]\w*) value;\s*value\.read\(stream\)/s', $translated, $valueType) === 1) {
                $objectType = ['type' => $valueType[1], 'pointer' => false];
            }
        }
        if ($objectType === null) {
            return null;
        }

        $target = $this->expression($assignment->var, $declaration, $method);
        $readTarget = $target;
        $assignmentType = $this->assignmentCPPType($assignment->var, $declaration);
        if ($assignmentType !== null && str_contains($assignmentType, 'std::optional<std::shared_ptr<')) {
            $readTarget .= '.value()';
        }
        $optionalValue = $assignmentType !== null
            && preg_match('/^std::optional<([^>]+)>$/', $assignmentType, $optionalType) === 1
            && !str_starts_with($optionalType[1], 'std::shared_ptr<');
        $local = $assignment->var instanceof Node\Expr\Variable;
        if ($objectType['pointer']) {
            return ($local ? 'auto ' . $target . ' =' : $target . ' =')
                . ' std::make_shared<' . $objectType['type'] . '>();' . "\n"
                . $readTarget . '->read(stream);';
        }
        if ($optionalValue) {
            return $target . '.emplace();' . "\n"
                . $target . '.value().read(stream);';
        }
        return ($local ? 'auto ' . $target . ' = ' . $objectType['type'] . '{};' : $target . '.read(stream);')
            . ($local ? "\n" . $target . '.read(stream);' : '');
    }

    /** @return array{type: string, pointer: bool}|null */
    private function objectTypeFromCppType(?string $cppType): ?array
    {
        if ($cppType === null || $cppType === '') {
            return null;
        }
        if (preg_match('/^std::optional<(.+)>$/', $cppType, $optionalType) === 1) {
            $cppType = $optionalType[1];
        }
        if (preg_match('/^std::shared_ptr<([A-Za-z_]\w*)>$/', $cppType, $pointerType) === 1) {
            $referenced = $this->findDeclaration($pointerType[1]);
            if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
                return null;
            }
            return ['type' => $pointerType[1], 'pointer' => true];
        }
        if (preg_match('/^[A-Za-z_]\w*$/', $cppType) !== 1
            || in_array($cppType, [
                'bool', 'char', 'int8_t', 'uint8_t', 'int16_t', 'uint16_t',
                'int32_t', 'uint32_t', 'int64_t', 'uint64_t', 'float', 'double',
                'std::string', 'std::string_view',
            ], true)) {
            return null;
        }
        $referenced = $this->findDeclaration($cppType);
        if ($referenced === null || $this->isEnumLikeDeclaration($referenced)) {
            return null;
        }
        return ['type' => $cppType, 'pointer' => false];
    }

    private function isDeserializerExpression(Node $expression): bool
    {
        if ($expression instanceof Node\Expr\StaticCall
            && $expression->class instanceof Node\Name
            && $expression->name instanceof Node\Identifier) {
            $class = $expression->class->getLast();
            $name = strtolower($expression->name->toString());
            return str_starts_with($name, 'get')
                || str_starts_with($name, 'read')
                || ($this->catalog->isSerializerClass($class) && str_starts_with($name, 'decode'));
        }
        if ($expression instanceof Node\Expr\MethodCall
            && $expression->name instanceof Node\Identifier) {
            $name = strtolower($expression->name->toString());
            return str_starts_with($name, 'get') || str_starts_with($name, 'read');
        }
        return false;
    }

private function sourceCodecBody(array $declaration, bool $read, bool $returnShared): ?string
    {
        if ($read && !$returnShared) {
            return null;
        }
        $target = CPPName::type((string) $declaration['name']);
        $codec = $this->findSourceCodec($target, $read);
        if ($codec === null) {
            return null;
        }

        $previousTypes = $this->expressionTypes;
        $previousAliases = $this->expressionAliases;
        $previousOptionals = $this->expressionOptionals;
        $unsupportedBefore = count($this->unsupported);
        $this->expressionTypes = [];
        $this->expressionAliases = [];
        $this->expressionOptionals = [];

        if (!$read) {
            $valueParameter = $codec['method']['parameters'][1]['name'] ?? null;
            if (is_string($valueParameter) && $valueParameter !== '') {
                $this->expressionTypes[$valueParameter] = $target;
                $this->expressionAliases[$valueParameter] = '(*this)';
            }
        }

        $lines = [];
        foreach ($codec['method']['body'] ?? [] as $statement) {
            $translated = $this->statement(
                $statement,
                $codec['owner'],
                $codec['method'],
                2,
                $read && $returnShared,
            );
            if ($translated !== '') {
                $lines[] = $translated;
            }
        }

        $this->expressionTypes = $previousTypes;
        $this->expressionAliases = $previousAliases;
        $this->expressionOptionals = $previousOptionals;

        if ($lines === [] || count($this->unsupported) !== $unsupportedBefore || str_contains(implode("\n", $lines), 'std::monostate')) {
            return null;
        }
        $body = implode("\n", $lines);
        if (str_contains($body, 'std::monostate')) {
            $fallback = $this->genericBody($declaration, $read, $returnShared);
            if (!str_contains($fallback, 'std::monostate')) {
                return $fallback;
            }
        }
        return $body;
    }

private function findSourceCodec(string $target, bool $read): ?array
    {
        foreach ($this->declarations as $owner) {
            foreach ($owner['methods'] ?? [] as $method) {
                if (!$method['static']) {
                    continue;
                }
                $methodName = strtolower((string) $method['name']);
                if ($read) {
                    if (!str_starts_with($methodName, 'get') && !str_starts_with($methodName, 'read')) {
                        continue;
                    }
                    if (CPPName::type((string) ($method['returnType'] ?? '')) !== $target) {
                        continue;
                    }
                    if (!$this->hasReaderParameter($method)) {
                        continue;
                    }
                    return ['owner' => $owner, 'method' => $method];
                }

                if (!str_starts_with($methodName, 'put') && !str_starts_with($methodName, 'write')) {
                    continue;
                }
                foreach (array_slice($method['parameters'] ?? [], 1) as $parameter) {
                    if (CPPName::type((string) ($parameter['type'] ?? '')) === $target) {
                        return ['owner' => $owner, 'method' => $method];
                    }
                }
            }
        }
        return null;
    }

private function hasReaderParameter(array $method): bool
    {
        $type = CPPName::type((string) ($method['parameters'][0]['type'] ?? ''));
        return str_ends_with($type, 'Reader') || str_contains($type, 'ReadOnly');
    }
}
