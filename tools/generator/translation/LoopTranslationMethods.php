<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait LoopTranslationMethods
{
private function listLoop(Node\Stmt\For_ $statement, array $declaration, array $method): ?string
    {
        if (count($statement->init) < 2 || count($statement->cond) !== 1 || count($statement->loop) !== 1 || count($statement->stmts) !== 1) {
            return null;
        }
        $indexInit = null;
        $countInit = null;
        $accumulatorInit = null;
        foreach ($statement->init as $initializer) {
            if (!$initializer instanceof Node\Expr\Assign || !$initializer->var instanceof Node\Expr\Variable) {
                continue;
            }
            if ($initializer->expr instanceof Node\Scalar\Int_ && $initializer->expr->value === 0 && $indexInit === null) {
                $indexInit = $initializer;
                continue;
            }
            if ($initializer->expr instanceof Node\Expr\Array_ && $initializer->expr->items === [] && $accumulatorInit === null) {
                $accumulatorInit = $initializer;
                continue;
            }
            if ($countInit === null) {
                $countInit = $initializer;
            }
        }
        $condition = $statement->cond[0];
        $increment = $statement->loop[0];
        $body = $statement->stmts[0];
        if (!$indexInit instanceof Node\Expr\Assign
            || !$condition instanceof Node\Expr\BinaryOp\Smaller
            || !$condition->left instanceof Node\Expr\Variable
            || !$condition->right instanceof Node\Expr\Variable
            || $condition->left->name !== $indexInit->var->name
            || (!$increment instanceof Node\Expr\PostInc && !$increment instanceof Node\Expr\PreInc)
            || !$increment->var instanceof Node\Expr\Variable
            || $increment->var->name !== $indexInit->var->name
            || !$body instanceof Node\Stmt\Expression
            || !$body->expr instanceof Node\Expr\Assign
            || !$body->expr->var instanceof Node\Expr\ArrayDimFetch
            || $body->expr->var->dim !== null) {
            return null;
        }
        if ($body->expr->expr instanceof Node\Expr\MethodCall
            && $body->expr->expr->var instanceof Node\Expr\Variable
            && $body->expr->expr->var->name === 'this') {
            return null;
        }
        $sourceCountName = CPPName::identifier((string) $condition->right->name);
        if ($countInit instanceof Node\Expr\Assign) {
            if (!$countInit->var instanceof Node\Expr\Variable || $countInit->var->name !== $condition->right->name) {
                return null;
            }
            $count = $this->expression($countInit->expr, $declaration, $method);
            if (!str_contains($count, 'stream.read')) {
                return null;
            }
        } else {
            $count = $sourceCountName;
        }
        $target = $this->expression($body->expr->var->var, $declaration, $method);
        $valueType = $this->appendElementCPPType($body->expr->var->var, $declaration);
        $value = $this->expression($body->expr->expr, $declaration, $method, $valueType);
        $countName = $countInit instanceof Node\Expr\Assign
            ? $this->loopCountName($target)
            : $sourceCountName;
        $index = CPPName::identifier((string) $indexInit->var->name);
        $incrementText = $this->expression($increment, $declaration, $method);
        $lines = [];
        if (!$this->isGeneratedObjectProperty($body->expr->var->var, $declaration)
            && !$this->isVectorExpression($body->expr->var->var, $declaration)) {
            $lines[] = 'auto ' . $target . ' = std::vector<decltype(' . $value . ')>();';
        }
        if ($countInit instanceof Node\Expr\Assign) {
            $lines[] = 'auto ' . $countName . ' = ' . $count . ';';
        }
        $lines[] = 'for (std::size_t ' . $index . ' = 0; ' . $index . ' < ' . $countName . '; ' . $incrementText . ') {';
        $lines[] = '    ' . $target . '.push_back(' . $value . ');';
        $lines[] = '}';
        return implode("\n", $lines);
    }

private function loopCountName(string $target): string
    {
        $parts = array_values(array_filter(
            preg_split('/[^A-Za-z0-9_]+/', $target) ?: [],
            static fn (string $part): bool => $part !== '',
        ));
        $base = $parts === [] ? 'count' : (string) end($parts);
        return CPPName::identifier($base . 'Count');
    }

private function forInitializer(array $expressions, array $declaration, array $method): array
    {
        $parts = [];
        $prefix = [];
        $declared = false;
        foreach ($expressions as $expression) {
            if ($expression instanceof Node\Expr\Assign && $expression->var instanceof Node\Expr\Variable && $expression->var->name !== 'this') {
                $name = $this->variableName((string) $expression->var->name, $declaration);
                $value = $this->expression($expression->expr, $declaration, $method);
                $inferredType = $this->inferredCPPType($expression->expr, $declaration);
                if ($inferredType !== null) {
                    $this->expressionTypes[(string) $expression->var->name] = $inferredType;
                }
                if (str_contains($value, 'stream.read')) {
                    $prefix[] = 'auto ' . $name . ' = ' . $value . ';';
                    continue;
                }
                $parts[] = ($declared ? $name : 'std::size_t ' . $name) . ' = ' . $value;
                $declared = true;
                continue;
            }
            $parts[] = $this->expression($expression, $declaration, $method);
        }
        return [implode(', ', $parts), $prefix];
    }
}
