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
            $lines[] = "\t\tstd::remove_cvref_t<decltype(" . $value . ")> " . $this->variableName($name, $declaration) . "{};";
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
                $value = $this->inferredPropertyReadValue($declaration, $property)
                    ?? $this->fallbackReadValue(
                        $property['type'] ?? null,
                        $property['doc'] ?? null,
                        $declaration,
                    );
                $enumType = $this->propertyEnumType($property, $declaration);
                if ($enumType !== null) {
                    $value = $this->stringEnumName($property, $declaration) !== null
                        ? $this->stringEnumFromStringForProperty($property, $value, $declaration)
                        : 'static_cast<' . $enumType . '>(' . $value . ')';
                }
                $lines[] = '        ' . $target . ' = ' . $value . ';';
            } elseif (!$returnShared) {
                $value = $field;
                $enumType = $this->propertyEnumType($property, $declaration);
                if ($enumType !== null) {
                    $value = $this->stringEnumName($property, $declaration) !== null
                        ? $this->stringEnumToStringForProperty($property, $value, $declaration)
                        : 'magic_enum::enum_integer(' . $value . ')';
                }
                $lines[] = '        ' . ($this->inferredPropertyWriteValue($declaration, $property, $value) ??
                    $this->fallbackWriteValue(
                        $property['type'] ?? null,
                        $property['doc'] ?? null,
                        $value,
                        $declaration,
                    ));
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
