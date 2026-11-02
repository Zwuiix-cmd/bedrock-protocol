<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\parser;

use PhpParser\Node;
use PhpParser\Node\Expr;
use PhpParser\NodeFinder;
use RuntimeException;

trait ConstantResolutionMethods
{
    private function resolveConstantValues(): void
    {
        foreach ($this->declarations as $fqcn => $declaration) {
            foreach ($declaration['constants'] as $constant) {
                $this->constants[$fqcn][$constant['name']] = $this->evaluate($constant['valueNode'], $fqcn);
            }
            foreach ($declaration['cases'] as $case) {
                $this->constants[$fqcn][$case['name']] = $case['valueNode'] === null ? null : $this->evaluate($case['valueNode'], $fqcn);
            }
        }
        foreach ($this->declarations as $fqcn => &$declaration) {
            foreach ($declaration['constants'] as &$constant) {
                $constant['value'] = $this->constants[$fqcn][$constant['name']] ?? null;
            }
            foreach ($declaration['properties'] as &$property) {
                $property['defaultValue'] = $property['defaultNode'] === null ? null : $this->evaluate($property['defaultNode'], $fqcn);
            }
            foreach ($declaration['cases'] as &$case) {
                $case['value'] = $this->constants[$fqcn][$case['name']] ?? null;
            }
            if (isset($declaration['networkIdNode'])) {
                $declaration['networkId'] = $this->evaluate($declaration['networkIdNode'], $fqcn);
            }
        }
        unset($declaration, $constant, $case);
        $this->resolveTraits();
    }

    private function resolveTraits(): void
    {
        foreach ($this->declarations as $key => &$declaration) {
            $traits = $declaration['traits'] ?? [];
            if ($traits === []) {
                continue;
            }
            foreach ($traits as $traitName) {
                $trait = $this->findDeclarationName((string) $traitName);
                if ($trait === null || ($this->declarations[$trait]['kind'] ?? null) !== 'trait') {
                    continue;
                }
                $source = $this->declarations[$trait];
                foreach (['constants', 'properties', 'methods'] as $member) {
                    $existing = array_map(static fn (array $item): string => (string) ($item['name'] ?? ''), $declaration[$member] ?? []);
                    foreach ($source[$member] ?? [] as $item) {
                        if (!in_array((string) ($item['name'] ?? ''), $existing, true)) {
                            $declaration[$member][] = $item;
                        }
                    }
                }
            }
        }
        unset($declaration);
    }

    private function evaluate(?Node $node, string $context): int|string|float|bool|null|array
    {
        if ($node === null) {
            return null;
        }
        if ($node instanceof Node\Scalar\Int_ || $node instanceof Node\Scalar\Float_ || $node instanceof Node\Scalar\String_) {
            return $node->value;
        }
        if ($node instanceof Expr\ConstFetch) {
            return match (strtolower($node->name->toString())) {
                'true' => true,
                'false' => false,
                'null' => null,
                default => null,
            };
        }
        if ($node instanceof Expr\ClassConstFetch) {
            $class = $node->class instanceof Node\Name ? $node->class->toString() : '';
            if ($class === 'self' || $class === 'static') {
                $class = $context;
            }
            $class = $this->findDeclarationName($class) ?? $class;
            $name = $node->name instanceof Node\Identifier ? $node->name->toString() : '';
            if (isset($this->constants[$class]) && array_key_exists($name, $this->constants[$class])) {
                $value = $this->constants[$class][$name];
                if ($value !== null) {
                    return $value;
                }
                $declaration = $this->declarations[$class] ?? null;
                if ($declaration !== null) {
                    foreach (array_merge($declaration['constants'], $declaration['cases']) as $constant) {
                        if ($constant['name'] === $name && $constant['valueNode'] !== $node) {
                            $resolved = $this->evaluate($constant['valueNode'], $class);
                            $this->constants[$class][$name] = $resolved;
                            return $resolved;
                        }
                    }
                }
            }
            return null;
        }
        if ($node instanceof Expr\UnaryMinus || $node instanceof Expr\UnaryPlus || $node instanceof Expr\BitwiseNot) {
            $value = $this->evaluate($node->expr, $context);
            if (!is_int($value) && !is_float($value)) {
                return null;
            }
            return $node instanceof Expr\UnaryMinus ? -$value : ($node instanceof Expr\UnaryPlus ? +$value : ~$value);
        }
        if ($node instanceof Expr\BinaryOp) {
            $left = $this->evaluate($node->left, $context);
            $right = $this->evaluate($node->right, $context);
            if ($left === null || $right === null) {
                return null;
            }
            return match (true) {
                $node instanceof Expr\BinaryOp\Plus => $left + $right,
                $node instanceof Expr\BinaryOp\Minus => $left - $right,
                $node instanceof Expr\BinaryOp\Mul => $left * $right,
                $node instanceof Expr\BinaryOp\Div && $right != 0 => $left / $right,
                $node instanceof Expr\BinaryOp\Mod && $right != 0 => $left % $right,
                $node instanceof Expr\BinaryOp\BitwiseOr => $left | $right,
                $node instanceof Expr\BinaryOp\BitwiseAnd => $left & $right,
                $node instanceof Expr\BinaryOp\BitwiseXor => $left ^ $right,
                $node instanceof Expr\BinaryOp\ShiftLeft => $left << $right,
                $node instanceof Expr\BinaryOp\ShiftRight => $left >> $right,
                $node instanceof Expr\BinaryOp\Concat => (string) $left . (string) $right,
                default => null,
            };
        }
        if ($node instanceof Expr\Array_) {
            $result = [];
            foreach ($node->items as $item) {
                if ($item === null) {
                    continue;
                }
                $key = $item->key === null ? count($result) : $this->evaluate($item->key, $context);
                $result[$key] = $this->evaluate($item->value, $context);
            }
            return $result;
        }
        return null;
    }

    private function findDeclarationName(string $name): ?string
    {
        $name = ltrim($name, '\\');
        if (isset($this->declarations[$name])) {
            return $name;
        }
        foreach ($this->declarations as $fqcn => $declaration) {
            if ($declaration['name'] === $name || str_ends_with($fqcn, '\\' . $name)) {
                return $fqcn;
            }
        }
        return null;
    }

    private function isPacket(array $declaration): bool
    {
        if ($declaration['kind'] !== 'class' || $declaration['abstract'] || !isset($declaration['networkIdNode'])) {
            return false;
        }
        $parent = $declaration['parent'];
        $seen = [];
        while ($parent !== null && $parent !== '') {
            $parentShort = basename(str_replace('\\', '/', $parent));
            if ($parentShort === 'DataPacket') {
                return true;
            }
            if (isset($seen[$parent])) {
                break;
            }
            $seen[$parent] = true;
            $parentDeclaration = $this->declarations[$this->findDeclarationName($parent) ?? $parent] ?? null;
            $parent = $parentDeclaration['parent'] ?? null;
        }
        return false;
    }

    private function findPoolRegistrations(): array
    {
        $pool = null;
        foreach ($this->declarations as $declaration) {
            if ($declaration['name'] === 'PacketPool') {
                $pool = $declaration;
                break;
            }
        }
        if ($pool === null) {
            return [];
        }
        $packetNames = [];
        foreach ($this->declarations as $declaration) {
            if ($this->isPacket($declaration)) {
                $packetNames[$declaration['name']] = $declaration['name'];
            }
        }
        $finder = new NodeFinder();
        $result = [];
        foreach ($pool['methods'] as $method) {
            foreach ($finder->findInstanceOf($method['body'] ?? [], Expr\New_::class) as $new) {
                $name = $new->class instanceof Node\Name ? $new->class->getLast() : null;
                if ($name !== null && isset($packetNames[$name]) && !in_array($name, $result, true)) {
                    $result[] = $name;
                }
            }
        }
        return $result;
    }

    private function protocolConstants(): array
    {
        foreach ($this->declarations as $declaration) {
            if ($declaration['name'] !== 'ProtocolInfo') {
                continue;
            }
            $result = [];
            foreach ($declaration['constants'] as $constant) {
                $result[$constant['name']] = $constant['value'];
            }
            return $result;
        }
        return [];
    }
}
