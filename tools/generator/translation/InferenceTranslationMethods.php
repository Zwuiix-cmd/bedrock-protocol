<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait InferenceTranslationMethods
{
private function inferredCPPType(Node $expression, array $declaration): ?string
    {
        if ($expression instanceof Node\Expr\Variable) {
            $name = (string) $expression->name;
            if (isset($this->expressionTypes[$name])) {
                return $this->cppType($this->expressionTypes[$name]);
            }
            for ($index = count($this->arrayScopes) - 1; $index >= 0; --$index) {
                if (isset($this->arrayScopes[$index][$name])) {
                    return 'std::vector<' . $this->arrayScopes[$index][$name] . '>';
                }
            }
        }
        if ($expression instanceof Node\Scalar\Int_) {
            return 'int64_t';
        }
        if ($expression instanceof Node\Scalar\Float_) {
            return 'float';
        }
        if ($expression instanceof Node\Scalar\String_) {
            return 'std::string';
        }
        if ($expression instanceof Node\Expr\BinaryOp) {
            if ($expression instanceof Node\Expr\BinaryOp\Equal
                || $expression instanceof Node\Expr\BinaryOp\Identical
                || $expression instanceof Node\Expr\BinaryOp\NotEqual
                || $expression instanceof Node\Expr\BinaryOp\NotIdentical
                || $expression instanceof Node\Expr\BinaryOp\Greater
                || $expression instanceof Node\Expr\BinaryOp\GreaterOrEqual
                || $expression instanceof Node\Expr\BinaryOp\Smaller
                || $expression instanceof Node\Expr\BinaryOp\SmallerOrEqual
                || $expression instanceof Node\Expr\BinaryOp\BooleanAnd
                || $expression instanceof Node\Expr\BinaryOp\BooleanOr) {
                return 'bool';
            }
            $left = $this->inferredCPPType($expression->left, $declaration);
            $right = $this->inferredCPPType($expression->right, $declaration);
            if ($left !== null && $right !== null && $left === $right) {
                return $left;
            }
            if ($left !== null && in_array($left, ['float', 'double'], true)) {
                return $left;
            }
            if ($right !== null && in_array($right, ['float', 'double'], true)) {
                return $right;
            }
            if ($left !== null || $right !== null) {
                return 'int64_t';
            }
        }
        if ($expression instanceof Node\Expr\ConstFetch) {
            return match (strtolower($expression->name->toString())) {
                'true', 'false' => 'bool',
                default => null,
            };
        }
        if ($expression instanceof Node\Expr\FuncCall
            && $expression->name instanceof Node\Name
            && in_array(strtolower($expression->name->toString()), ['count', 'sizeof', 'strlen'], true)) {
            return 'std::size_t';
        }
        if ($expression instanceof Node\Expr\ClassConstFetch) {
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            $referenced = $class === 'self' || $class === 'static'
                ? $declaration
                : $this->findDeclaration($class);
            foreach ($referenced['constants'] ?? [] as $constant) {
                if (($constant['name'] ?? '') !== $name) {
                    continue;
                }
                return match (true) {
                    is_float($constant['value'] ?? null) => 'float',
                    is_string($constant['value'] ?? null) => 'std::string',
                    default => 'int64_t',
                };
            }
            if ($class === 'self' || $class === 'static') {
                return 'int64_t';
            }
            return null;
        }
        if ($expression instanceof Node\Expr\MethodCall
            && $expression->var instanceof Node\Expr\Variable
            && in_array($expression->var->name, ['in', 'out', 'connRequestReader', 'connRequestWriter'], true)) {
            $name = $expression->name instanceof Node\Identifier ? $expression->name->toString() : '';
            $codec = $this->codecForCall('stream', $name);
            return $codec['type'] ?? null;
        }
        if ($expression instanceof Node\Expr\MethodCall
            && $expression->name instanceof Node\Identifier
            && strtolower($expression->name->toString()) === 'size') {
            return 'std::size_t';
        }
        if ($expression instanceof Node\Expr\PropertyFetch || $expression instanceof Node\Expr\ArrayDimFetch || $expression instanceof Node\Expr\MethodCall) {
            $type = $this->expressionType($expression, $declaration);
            return $type === null ? null : $this->cppType($type);
        }
        if ($expression instanceof Node\Expr\StaticCall) {
            $name = $expression->name instanceof Node\Identifier ? strtolower($expression->name->toString()) : '';
            $class = $expression->class instanceof Node\Name ? $expression->class->getLast() : '';
            if ($class === 'Color' && in_array($name, ['fromargb', 'fromrgba'], true)) {
                return 'uint32_t';
            }
            if ($class === 'Binary' && $name === 'flipintendianness') {
                return 'uint32_t';
            }
            if ($this->catalog->isSerializerClass($class)) {
                $codec = $this->codecForCall($class, $name);
                if ($codec !== null) {
                    return $codec['type'];
                }
            }
            if (in_array($name, ['read', 'decode'], true)) {
                if ($class !== '' && $class !== 'self' && $class !== 'static') {
                    return 'std::shared_ptr<' . CPPName::type($class) . '>';
                }
            }
            if (in_array($name, ['frompacket', 'fromordinal'], true)
                && $expression->class instanceof Node\Name) {
                $class = $expression->class->getLast();
                $referenced = $this->findDeclaration($class);
                if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
                    return $class;
                }
            }
        }
        if ($expression instanceof Node\Expr\New_ && $expression->class instanceof Node\Name) {
            return 'std::shared_ptr<' . CPPName::type($expression->class->getLast()) . '>';
        }
        return null;
    }

private function cppTypeForInferredVariable(Node $expression, array $declaration): ?string
    {
        $enumType = $this->classConstantType($expression);
        if ($enumType !== null) {
            return $enumType;
        }
        return $this->inferredCPPType($expression, $declaration);
    }

private function inferArrayScope(array $statements, array $declaration): array
    {
        $types = [];
        foreach ($statements as $statement) {
            if ($statement instanceof Node\Stmt\Expression
                && $statement->expr instanceof Node\Expr\Assign
                && $statement->expr->var instanceof Node\Expr\ArrayDimFetch
                && $statement->expr->var->var instanceof Node\Expr\Variable) {
                $source = $statement->expr->expr;
                $type = $source instanceof Node\Expr\Variable && isset($types[(string) $source->name])
                    ? 'std::vector<' . $types[(string) $source->name] . '>'
                    : $this->inferredCPPType($source, $declaration);
                if ($type !== null) {
                    $types[(string) $statement->expr->var->var->name] = $type;
                }
                continue;
            }
            $children = $statement instanceof Node\Stmt\For_
                ? $statement->stmts
                : ($statement instanceof Node\Stmt\Foreach_ ? $statement->stmts : []);
            if ($children !== []) {
                $types = array_replace($types, $this->inferArrayScope($children, $declaration));
            }
        }
        return $types;
    }

private function emptyArrayValue(string $name): string
    {
        for ($index = count($this->arrayScopes) - 1; $index >= 0; --$index) {
            $type = $this->arrayScopes[$index][$name] ?? null;
            if ($type !== null) {
                return 'std::vector<' . $type . '>()';
            }
        }
        return 'std::vector<int32_t>()';
    }

private function nestedAssignmentNodes(array $statements): array
    {
        $assignments = [];
        foreach ($statements as $statement) {
            $this->collectNestedAssignmentNodes($statement, $assignments);
        }
        return $assignments;
    }

private function collectNestedAssignmentNodes(mixed $node, array &$assignments): void
    {
        if (!$node instanceof Node) {
            return;
        }
        if ($node instanceof Node\Stmt\For_) {
            foreach ($node->cond as $child) {
                $this->collectNestedAssignmentNodes($child, $assignments);
            }
            foreach ($node->loop as $child) {
                $this->collectNestedAssignmentNodes($child, $assignments);
            }
            foreach ($node->stmts as $child) {
                $this->collectNestedAssignmentNodes($child, $assignments);
            }
            return;
        }
        if ($node instanceof Node\Stmt\Expression
            && $node->expr instanceof Node\Expr\Assign
            && $node->expr->var instanceof Node\Expr\Variable
            && $node->expr->var->name !== 'this') {
            $this->collectNestedAssignmentNodes($node->expr->expr, $assignments);
            return;
        }
        if ($node instanceof Node\Expr\Assign
            && $node->var instanceof Node\Expr\Variable
            && $node->var->name !== 'this') {
            $assignments[(string) $node->var->name] ??= $node;
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->{$name};
            if ($child instanceof Node) {
            $this->collectNestedAssignmentNodes($child, $assignments);
            } elseif (is_array($child)) {
                foreach ($child as $value) {
                    if ($value instanceof Node) {
                        $this->collectNestedAssignmentNodes($value, $assignments);
                    }
                }
            }
        }
    }

private function templateArguments(?string $type, string $template): array
    {
        if ($type === null || !str_starts_with($type, $template . '<') || !str_ends_with($type, '>')) {
            return [];
        }
        $contents = substr($type, strlen($template) + 1, -1);
        $arguments = [];
        $depth = 0;
        $start = 0;
        for ($index = 0, $length = strlen($contents); $index < $length; ++$index) {
            $character = $contents[$index];
            if ($character === '<') {
                ++$depth;
            } elseif ($character === '>') {
                --$depth;
            } elseif ($character === ',' && $depth === 0) {
                $arguments[] = trim(substr($contents, $start, $index - $start));
                $start = $index + 1;
            }
        }
        $last = trim(substr($contents, $start));
        if ($last !== '') {
            $arguments[] = $last;
        }
        return $arguments;
    }
}
