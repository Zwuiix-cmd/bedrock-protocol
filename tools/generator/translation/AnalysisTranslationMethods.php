<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait AnalysisTranslationMethods
{
private function findDeclaration(string $shortName): ?array
    {
        foreach ($this->declarations as $declaration) {
            if ($declaration['name'] === $shortName && $this->isGeneratedDeclaration($declaration)) {
                return $declaration;
            }
        }
        return null;
    }

private function hasMethod(array $declaration, string $name, bool $static): bool
    {
        foreach ($declaration['methods'] as $method) {
            if ($method['name'] === $name && $method['static'] === $static) {
                return true;
            }
        }
        return false;
    }

private function isEnumLikeDeclaration(array $declaration): bool
    {
        if (($declaration['kind'] ?? null) === 'enum') {
            return true;
        }
        if (($declaration['kind'] ?? null) !== 'class' || ($declaration['properties'] ?? []) !== []) {
            return false;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['name'] ?? '') !== '__construct') {
                return false;
            }
        }
        $constants = array_values(array_filter(
            $declaration['constants'] ?? [],
            static fn (array $constant): bool => ($constant['name'] ?? '') !== 'NETWORK_ID',
        ));
        if ($constants === []) {
            return false;
        }
        foreach ($constants as $constant) {
            if (!is_int($constant['value'] ?? null)
                && !is_bool($constant['value'] ?? null)
                && !is_string($constant['value'] ?? null)) {
                return false;
            }
        }
        return true;
    }

private function isGeneratedDeclaration(array $declaration): bool
    {
        if ($this->isEnumLikeDeclaration($declaration)) {
            return true;
        }
        if (!in_array($declaration['kind'], ['class', 'interface'], true)) {
            return false;
        }
        if ($this->isRuntimeDeclaration((string) $declaration['name'])) {
            return false;
        }
        return str_contains($declaration['namespace'], '\\types') || str_starts_with($declaration['file'], 'src/types/');
    }

private function isRuntimeDeclaration(string $name): bool
    {
        return $this->catalog->isRuntimeDeclaration($name);
    }

private function isGeneratedObjectProperty(Node $node, array $declaration): bool
    {
        if (!$node instanceof Node\Expr\PropertyFetch || !$node->var instanceof Node\Expr\Variable || $node->var->name !== 'this') {
            return false;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        foreach ($declaration['properties'] as $property) {
            if ($property['name'] !== $name) {
                continue;
            }
            $referenced = $this->findDeclaration(CPPName::type((string) $property['type']));
            return $referenced !== null && $referenced['kind'] === 'class';
        }
        return false;
    }

private function isNullExpression(?Node $node): bool
    {
        return $node instanceof Node\Expr\ConstFetch && strtolower($node->name->toString()) === 'null';
    }

private function isOptionalProperty(Node $node, array $declaration): bool
    {
        if (!$node instanceof Node\Expr\PropertyFetch) {
            return false;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        if ($node->var instanceof Node\Expr\Variable && $node->var->name === 'this') {
            $properties = $declaration['properties'];
        } else {
            $objectType = $this->expressionType($node->var, $declaration);
            $referenced = $objectType === null ? null : $this->findDeclaration(CPPName::type($objectType));
            $properties = $referenced['properties'] ?? [];
        }
        foreach ($properties as $property) {
            if (($property['name'] ?? null) === $name) {
                return $this->nullableType((string) ($property['type'] ?? ''))
                    || $this->nullableType($this->docType($property['doc'] ?? null) ?? '');
            }
        }
        return false;
    }

private function isOptionalContainer(Node $node, array $declaration): bool
    {
        if ($this->isOptionalProperty($node, $declaration)) {
            return true;
        }
        if ($node instanceof Node\Expr\Variable) {
            return $this->expressionOptionals[(string) $node->name] ?? false;
        }
        if ($node instanceof Node\Expr\ArrayDimFetch) {
            return $this->isOptionalContainer($node->var, $declaration);
        }
        return false;
    }

private function isOptionalExpression(Node $node): bool
    {
        if ($node instanceof Node\Expr\Variable) {
            return $this->expressionOptionals[(string) $node->name] ?? false;
        }
        if ($node instanceof Node\Expr\StaticCall || $node instanceof Node\Expr\MethodCall) {
            $name = $node->name instanceof Node\Identifier ? strtolower($node->name->toString()) : '';
            return $name === 'readoptional';
        }
        if ($node instanceof Node\Expr\Ternary) {
            return $this->isNullExpression($node->if) || $this->isNullExpression($node->else);
        }
        return false;
    }

private function isVoidExpression(Node\Expr $expression): bool
    {
        if ($expression instanceof Node\Expr\MethodCall || $expression instanceof Node\Expr\NullsafeMethodCall) {
            $name = $expression->name instanceof Node\Identifier ? strtolower($expression->name->toString()) : '';
            return str_starts_with($name, 'write') || str_starts_with($name, 'put');
        }
        if ($expression instanceof Node\Expr\StaticCall) {
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            $name = $expression->name instanceof Node\Identifier ? strtolower($expression->name->toString()) : '';
            return $this->catalog->isSerializerClass($class)
                && (str_starts_with($name, 'write') || str_starts_with($name, 'put'));
        }
        return false;
    }

private function isVectorExpression(Node $node, array $declaration): bool
    {
        if (!$node instanceof Node\Expr\PropertyFetch || !$node->var instanceof Node\Expr\Variable || $node->var->name !== 'this') {
            return false;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        foreach ($declaration['properties'] as $property) {
            if (($property['name'] ?? null) !== $name) {
                continue;
            }
            $cppType = $this->cppPropertyType($property, $declaration);
            if (str_starts_with($cppType, 'std::map<') || str_starts_with($cppType, 'std::unordered_map<')) {
                return false;
            }
            $type = (string) ($property['type'] ?? '');
            $doc = $this->docType($property['doc'] ?? null);
            return $type === 'array' || str_ends_with($type, '[]') || $this->arrayElementType($type, $doc) !== null;
        }
        return false;
    }

private function propertyUsesAppend(array $property, array $declaration): bool
    {
        $name = (string) ($property['name'] ?? '');
        if ($name === '') {
            return false;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            foreach ($method['body'] ?? [] as $statement) {
                if ($this->nodeUsesPropertyAppend($statement, $name)) {
                    return true;
                }
            }
        }
        return false;
    }

private function emittedExpressionType(Node\Expr $expression, array $declaration): ?string
    {
        if ($expression instanceof Node\Expr\PropertyFetch
            && $expression->var instanceof Node\Expr\Variable
            && $expression->var->name === 'this') {
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            foreach ($declaration['properties'] ?? [] as $property) {
                if (($property['name'] ?? null) === $name) {
                    return $this->cppPropertyType($property, $declaration);
                }
            }
        }
        if ($expression instanceof Node\Expr\PropertyFetch) {
            $objectType = $this->expressionType($expression->var, $declaration);
            $propertyName = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                foreach ($referenced['properties'] ?? [] as $property) {
                    if (($property['name'] ?? null) === $propertyName) {
                        return $this->cppPropertyType($property, $referenced);
                    }
                }
            }
        }
        $type = $this->expressionType($expression, $declaration);
        return $type === null ? null : $this->cppType($type);
    }

private function propertyUsesIndexedAccess(array $property, array $declaration): bool
    {
        $name = (string) ($property['name'] ?? '');
        if ($name === '') {
            return false;
        }
        foreach ($declaration['methods'] ?? [] as $method) {
            foreach ($method['body'] ?? [] as $statement) {
                if ($this->nodeUsesPropertyIndex($statement, $name)) {
                    return true;
                }
            }
        }
        return false;
    }

private function nodeUsesPropertyIndex(mixed $node, string $propertyName): bool
    {
        if (!$node instanceof Node) {
            return false;
        }
        if ($node instanceof Node\Expr\ArrayDimFetch
            && $node->dim !== null) {
            $variable = $node->var;
            if ($variable instanceof Node\Expr\PropertyFetch
                && $variable->var instanceof Node\Expr\Variable
                && $variable->var->name === 'this'
                && $variable->name instanceof Node\Identifier
                && $variable->name->toString() === $propertyName) {
                return true;
            }
            if ($variable instanceof Node\Expr\Variable && $variable->name === $propertyName) {
                return true;
            }
        }
        foreach ($node->getSubNodeNames() as $subNodeName) {
            $child = $node->{$subNodeName};
            if ($child instanceof Node && $this->nodeUsesPropertyIndex($child, $propertyName)) {
                return true;
            }
            if (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node && $this->nodeUsesPropertyIndex($item, $propertyName)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

private function nodeUsesPropertyAppend(mixed $node, string $propertyName): bool
    {
        if (!$node instanceof Node) {
            return false;
        }
        if ($node instanceof Node\Expr\ArrayDimFetch
            && $node->dim === null
            && $node->var instanceof Node\Expr\PropertyFetch
            && $node->var->var instanceof Node\Expr\Variable
            && $node->var->var->name === 'this'
            && $node->var->name instanceof Node\Identifier
            && $node->var->name->toString() === $propertyName) {
            return true;
        }
        foreach ($node->getSubNodeNames() as $subNodeName) {
            $child = $node->{$subNodeName};
            if ($child instanceof Node && $this->nodeUsesPropertyAppend($child, $propertyName)) {
                return true;
            }
            if (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node && $this->nodeUsesPropertyAppend($item, $propertyName)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

private function foreachElementType(Node\Expr $expression, array $declaration): ?string
    {
        if ($expression instanceof Node\Expr\PropertyFetch
            && $expression->var instanceof Node\Expr\Variable
            && $expression->var->name === 'this') {
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            foreach ($declaration['properties'] as $property) {
                if (($property['name'] ?? null) !== $name) {
                    continue;
                }
                return $this->arrayElementType($property['type'] ?? null, $property['doc'] ?? null);
            }
        }
        if ($expression instanceof Node\Expr\Variable) {
            return $this->expressionTypes[(string) $expression->name] ?? null;
        }
        if ($expression instanceof Node\Expr\MethodCall) {
            $objectType = $this->expressionType($expression->var, $declaration);
            if ($objectType !== null) {
                $referenced = $this->findDeclaration(CPPName::type($objectType));
                $methodName = $expression->name instanceof Node\Identifier ? strtolower($expression->name->toString()) : '';
                if ($referenced !== null && str_starts_with($methodName, 'get')) {
                    $suffix = substr($methodName, 3);
                    foreach ($referenced['properties'] ?? [] as $property) {
                        if (!$property['static'] && strtolower((string) $property['name']) === $suffix) {
                            return $this->arrayElementType($property['type'] ?? null, $property['doc'] ?? null);
                        }
                    }
                }
            }
        }
        return null;
    }

private function isUnknownProperty(Node $node, array $declaration): bool
    {
        if (!$node instanceof Node\Expr\PropertyFetch || !$node->var instanceof Node\Expr\Variable || $node->var->name !== 'this') {
            return false;
        }
        $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
        foreach ($declaration['properties'] as $property) {
            if ($property['name'] !== $name) {
                continue;
            }
            $type = (string) ($property['type'] ?? '');
            return $type === '' || $type === 'mixed' || $type === 'array' || str_contains($type, '|') || $this->cppType($type) === 'std::monostate';
        }
        return true;
    }

private function isStringEnumProperty(Node $node, array $declaration): bool
    {
        if ($this->stringEnumInfo($node, $declaration) !== null) {
            return true;
        }
        if ($node instanceof Node\Expr\Variable) {
            return false;
        }
        if (!$node instanceof Node\Expr\PropertyFetch || !$node->var instanceof Node\Expr\Variable || $node->var->name !== 'this') {
            return false;
        }
        return false;
    }

private function isKnownValueType(string $name): bool
    {
        return $this->catalog->isKnownValueType($name);
    }

private function recordUnsupported(array $declaration, array $method, string $reason): void
    {
        $entry = ['class' => $declaration['fqcn'], 'method' => $method['name'], 'line' => $method['line'], 'reason' => $reason];
        foreach ($this->unsupported as $existing) {
            if ($existing === $entry) {
                return;
            }
        }
        $this->unsupported[] = $entry;
    }

private function nestedAssignmentVariables(array $statements): array
    {
        $variables = [];
        foreach ($statements as $statement) {
            if ($statement instanceof Node\Stmt\Expression
                && $statement->expr instanceof Node\Expr\Assign
                && $statement->expr->var instanceof Node\Expr\Variable) {
                $this->collectNestedAssignments($statement->expr->expr, $variables);
                continue;
            }
            $this->collectNestedAssignments($statement, $variables);
        }
        return array_values(array_unique($variables));
    }

private function localVariableNames(array $statements): array
    {
        $variables = [];
        foreach ($statements as $statement) {
            $this->collectLocalVariables($statement, $variables);
        }
        return array_values(array_unique($variables));
    }

private function collectLocalVariables(Node $node, array &$variables): void
    {
        if ($node instanceof Node\Expr\Variable
            && is_string($node->name)
            && !in_array($node->name, ['this', 'in', 'out', 'connRequestReader', 'connRequestWriter'], true)) {
            $variables[] = $node->name;
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->$name;
            if ($child instanceof Node) {
                $this->collectLocalVariables($child, $variables);
            } elseif (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof Node) {
                        $this->collectLocalVariables($item, $variables);
                    }
                }
            }
        }
    }

private function variableName(string $name, array $declaration): string
    {
        $properties = array_column($declaration['properties'] ?? [], 'name');
        if (isset($this->localVariables[$name]) && in_array($name, $properties, true)) {
            return CPPName::identifier($name . 'Value');
        }
        return CPPName::identifier($name);
    }
}
