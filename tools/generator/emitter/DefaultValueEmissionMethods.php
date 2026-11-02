<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait DefaultValueEmissionMethods
{
    private function propertyAssignedBySerializer(array $declaration, string $propertyName, string $methodName): bool
    {
        foreach ($declaration['methods'] ?? [] as $method) {
            if (($method['static'] ?? false)
                || $this->canonicalMethodName((string) ($method['name'] ?? '')) !== $methodName) {
                continue;
            }
            foreach ($method['body'] ?? [] as $statement) {
                if ($this->nodeAssignsProperty($statement, $propertyName)) {
                    return true;
                }
            }
        }
        return false;
    }

    private function nodeAssignsProperty(mixed $node, string $propertyName): bool
    {
        if (!$node instanceof \PhpParser\Node) {
            return false;
        }
        if ($node instanceof \PhpParser\Node\Expr\Assign
            && $node->var instanceof \PhpParser\Node\Expr\PropertyFetch
            && $node->var->var instanceof \PhpParser\Node\Expr\Variable
            && $node->var->var->name === 'this'
            && $node->var->name instanceof \PhpParser\Node\Identifier
            && $node->var->name->toString() === $propertyName) {
            return true;
        }
        foreach ($node->getSubNodeNames() as $name) {
            $child = $node->{$name};
            if ($child instanceof \PhpParser\Node && $this->nodeAssignsProperty($child, $propertyName)) {
                return true;
            }
            if (is_array($child)) {
                foreach ($child as $item) {
                    if ($item instanceof \PhpParser\Node && $this->nodeAssignsProperty($item, $propertyName)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    private function propertyInitializer(array $property, array $declaration, string $cppType): ?string
    {
        $sourceType = (string) ($property['type'] ?? '');
        $nullable = str_starts_with($sourceType, '?') || str_contains($sourceType, '|null') || str_contains($sourceType, '| null');
        $defaultText = trim((string) ($property['defaultText'] ?? ''));
        $hasExplicitDefault = ($property['defaultNode'] ?? null) !== null || $defaultText !== '';
        if (!$hasExplicitDefault) {
            return null;
        }
        $stringEnumDefault = $this->stringEnumDefault($property, $declaration, $cppType);
        if ($stringEnumDefault !== null) {
            return $stringEnumDefault;
        }
        if ($nullable && ($defaultText === '' || strtolower($defaultText) === 'null')) {
            return null;
        }

        if ($defaultText !== '' && in_array(ltrim($sourceType, '?'), ['int', 'float', 'bool', 'string'], true)) {
            if (ltrim($sourceType, '?') === 'string' && ($property['defaultValue'] ?? null) === '') {
                return null;
            }
            if ($cppType === 'BuildPlatform') {
                return $property['defaultNode'] !== null
                    ? $this->translator->expression($property['defaultNode'], $declaration)
                    : 'static_cast<BuildPlatform>(' . (int) ($property['defaultValue'] ?? -1) . ')';
            }
            if (isset($this->enumNames[$cppType])) {
                return $property['defaultNode'] !== null
                    ? $this->translator->expression($property['defaultNode'], $declaration)
                    : 'static_cast<' . $cppType . '>(' . (int) ($property['defaultValue'] ?? 0) . ')';
            }
            return $this->defaultExpression($property, $declaration);
        }

        $typeName = CPPName::type($sourceType);
        if (isset($this->enumNames[$typeName])) {
            return CPPDefaultValueResolver::member($cppType, true);
        }
        return CPPDefaultValueResolver::member($cppType);
    }

    private function stringEnumDefault(array $property, array $declaration, string $cppType): ?string
    {
        if (ltrim((string) ($property['type'] ?? ''), '?') !== 'string') {
            return null;
        }
        $prefix = strtoupper((string) ($property['name'] ?? '')) . '_';
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (str_starts_with((string) ($constant['name'] ?? ''), $prefix) && is_string($constant['value'] ?? null)) {
                return $cppType . '::' . CPPName::identifier((string) $constant['value']);
            }
        }
        return null;
    }

    private function defaultExpression(array $property, array $declaration): string
    {
        if (str_starts_with((string) $property['type'], '?')
            && (($property['defaultValue'] ?? null) === null || trim((string) ($property['defaultText'] ?? '')) === 'null')) {
            return 'std::nullopt';
        }
        if (array_key_exists('defaultValue', $property)) {
            $value = $property['defaultValue'];
            if (is_bool($value)) {
                return $value ? 'true' : 'false';
            }
            if (is_int($value) || is_float($value)) {
                if (is_float($value)) {
                    $text = (string) $value;
                    if (!str_contains($text, '.') && !str_contains(strtolower($text), 'e')) {
                        $text .= '.0';
                    }
                    return $text . 'f';
                }
                return (string) $value;
            }
            if (is_string($value)) {
                return '"' . addcslashes($value, "\\\"") . '"';
            }
        }
        if ($property['defaultNode'] !== null) {
            return $this->translator->expression($property['defaultNode'], $declaration);
        }
        return $this->defaultValue((string) $property['defaultText']);
    }
}
