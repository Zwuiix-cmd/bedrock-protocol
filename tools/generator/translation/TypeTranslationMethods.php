<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait TypeTranslationMethods
{
public function cppType(?string $type, bool $stringView = false, ?string $doc = null): string
    {
        $type ??= $this->docType($doc) ?? 'mixed';
        if ($type === null || $type === '' || $type === 'mixed') {
            return 'std::monostate';
        }
        $nullable = str_starts_with($type, '?');
        $type = ltrim($type, '?');
        if (str_contains($type, '|')) {
            $rawParts = $this->splitTypeParts($type, '|');
            $nullable = $nullable || count($rawParts) !== count(array_filter($rawParts, static fn (string $part): bool => strtolower($part) !== 'null'));
            $parts = array_values(array_filter($rawParts, static fn (string $part): bool => strtolower($part) !== 'null'));
            if (count($parts) > 1) {
                $variantTypes = [];
                foreach ($parts as $part) {
                    $variantType = $this->cppType($part, $stringView);
                    if (!in_array($variantType, $variantTypes, true)) {
                        $variantTypes[] = $variantType;
                    }
                }
                if ($variantTypes === []) {
                    return 'std::monostate';
                }
                if (count($variantTypes) === 1) {
                    return $nullable ? 'std::optional<' . $variantTypes[0] . '>' : $variantTypes[0];
                }
                if ($nullable) {
                    array_unshift($variantTypes, 'std::monostate');
                }
                return 'std::variant<' . implode(', ', $variantTypes) . '>';
            }
            if (count($parts) === 1) {
                $nullable = true;
                $type = $parts[0];
            } else {
                return 'std::monostate';
            }
        }
        if (in_array(strtolower($type), ['int8', 'int8_t', 'byte'], true)) {
            $result = 'int8_t';
        } elseif (in_array(strtolower($type), ['uint8', 'uint8_t', 'unsigned byte'], true)) {
            $result = 'uint8_t';
        } elseif (in_array(strtolower($type), ['int16', 'int16_t', 'short'], true)) {
            $result = 'int16_t';
        } elseif (in_array(strtolower($type), ['uint16', 'uint16_t', 'unsigned short'], true)) {
            $result = 'uint16_t';
        } elseif (in_array(strtolower($type), ['int', 'int32', 'int32_t', 'signed int'], true)) {
            $result = 'int32_t';
        } elseif (in_array(strtolower($type), ['uint32', 'uint32_t', 'unsigned int'], true)) {
            $result = 'uint32_t';
        } elseif (in_array(strtolower($type), ['int64', 'int64_t', 'long', 'long long'], true)) {
            $result = 'int64_t';
        } elseif (in_array(strtolower($type), ['uint64', 'uint64_t', 'unsigned long', 'unsigned long long'], true)) {
            $result = 'uint64_t';
        } elseif ($type === 'float') {
            $result = 'float';
        } elseif ($type === 'bool') {
            $result = 'bool';
        } elseif ($type === 'string') {
            $result = $stringView ? 'std::string_view' : 'std::string';
        } elseif (str_ends_with($type, '::*') || str_contains($type, '::')) {
            $result = str_ends_with($type, '::*') ? 'std::string' : 'int32_t';
        } elseif ($type === 'array' || str_ends_with($type, '[]') || preg_match('/^(?:array|list)\s*</i', $type) === 1) {
            $parts = $this->arrayTypeParts($type, $doc);
            if ($parts === null) {
                $result = 'std::vector<std::monostate>';
            } elseif ($parts['key'] !== null) {
                $result = 'std::map<' . $this->cppType($parts['key']) . ', ' . $this->cppType($parts['value']) . '>';
            } else {
                $result = 'std::vector<' . $this->cppType($parts['value']) . '>';
            }
        } elseif (($alias = $this->catalog->typeAlias(CPPName::type($type))) !== null && !str_contains($alias, '<')) {
            $result = $alias;
        } elseif ($type === 'void') {
            $result = 'void';
        } elseif ($type === 'self' || $type === 'static') {
            $result = 'std::monostate';
        } else {
            $short = CPPName::type($type);
            $declaration = $this->findDeclaration($short);
            if ($declaration !== null && $this->isEnumLikeDeclaration($declaration)) {
                $result = $short;
            } elseif (($alias = $this->catalog->typeAlias($short)) !== null) {
                $result = $alias;
            } elseif ($this->isKnownValueType($short)) {
                $result = $short;
            } elseif ($declaration !== null) {
                $result = 'std::shared_ptr<' . $short . '>';
            } else {
                $result = 'std::monostate';
            }
        }
        return $nullable ? 'std::optional<' . $result . '>' : $result;
    }

public function cppPropertyType(array $property, array $declaration): string
    {
        $enumType = $this->propertyEnumType($property, $declaration);
        if ($enumType !== null) {
            return $this->nullableType((string) ($property['type'] ?? ''))
                ? 'std::optional<' . $enumType . '>'
                : $enumType;
        }
        $inferredType = $this->inferredPropertyType($declaration, $property);
        if ($inferredType !== null) {
            $sourceType = (string) ($property['type'] ?? '');
            $baseType = ltrim($sourceType, '?');
            $doc = (string) ($property['doc'] ?? '');
            $arrayParts = $this->arrayTypeParts($sourceType, $doc);
            if ($arrayParts !== null && $arrayParts['key'] === null) {
                $elementType = (string) $arrayParts['value'];
                $elementDeclaration = $this->findDeclaration(CPPName::type($elementType));
                $semanticElementType = $elementDeclaration !== null
                    && in_array($elementDeclaration['kind'] ?? null, ['class', 'enum', 'interface'], true)
                    ? $this->cppType($elementType)
                    : $inferredType;
                $result = 'std::vector<' . $semanticElementType . '>';
                return $this->nullableType($sourceType) ? 'std::optional<' . $result . '>' : $result;
            }
            if (in_array(strtolower($baseType), ['int', 'float', 'bool', 'string'], true)) {
                return $this->nullableType($sourceType)
                    ? 'std::optional<' . $inferredType . '>'
                    : $inferredType;
            }
        }
        $doc = (string) ($property['doc'] ?? '');
        $sourceType = $this->docType($doc) ?? (string) ($property['type'] ?? '');
        if (preg_match('/(?:array|list)\s*<\s*(?:[^,>]+\s*,\s*)?MetadataProperty\s*>/i', $doc) === 1) {
            return 'EntityMetadataCollection';
        }
        $arrayParts = $this->arrayTypeParts($sourceType, $doc);
        if ($arrayParts !== null
            && $arrayParts['key'] !== null
            && strtolower(ltrim($arrayParts['key'], '?\\')) === 'int'
            && !$this->propertyUsesIndexedAccess($property, $declaration)) {
            $element = $this->cppType($arrayParts['value']);
            $result = 'std::vector<' . $element . '>';
            return $this->nullableType($sourceType) ? 'std::optional<' . $result . '>' : $result;
        }
        return $this->cppType($property['type'] ?? null, false, $property['doc'] ?? null);
    }

private function nullableType(string $type): bool
    {
        if (str_starts_with($type, '?')) {
            return true;
        }
        foreach ($this->splitTypeParts($type, '|') as $part) {
            if (strtolower(trim($part)) === 'null') {
                return true;
            }
        }
        return false;
    }

private function withoutNullType(string $type): string
    {
        $parts = array_values(array_filter(array_map(static fn (string $part): string => ltrim(trim($part), '?'), $this->splitTypeParts($type, '|')), static fn (string $part): bool => strtolower($part) !== 'null'));
        return $parts[0] ?? 'mixed';
    }

private function splitTypeParts(string $type, string $separator): array
    {
        $parts = [];
        $depth = 0;
        $start = 0;
        for ($index = 0, $length = strlen($type); $index < $length; ++$index) {
            $character = $type[$index];
            if ($character === '<' || $character === '(' || $character === '[') {
                ++$depth;
            } elseif ($character === '>' || $character === ')' || $character === ']') {
                --$depth;
            } elseif ($character === $separator && $depth === 0) {
                $parts[] = trim(substr($type, $start, $index - $start));
                $start = $index + 1;
            }
        }
        $parts[] = trim(substr($type, $start));
        return $parts;
    }

private function unionParts(string $type): array
    {
        if (!str_contains($type, '|')) {
            return [];
        }
        return array_values(array_filter(
            array_map(static fn (string $part): string => ltrim(trim($part), '?'), $this->splitTypeParts($type, '|')),
            static fn (string $part): bool => $part !== '' && strtolower($part) !== 'null',
        ));
    }

private function arrayTypeParts(?string $type, ?string $doc): ?array
    {
        $candidate = $this->docType($doc) ?? $type;
        if ($candidate === null || $candidate === '') {
            return null;
        }
        $candidate = ltrim($candidate, '?');
        $union = $this->unionParts($candidate);
        if ($union !== []) {
            $candidate = count($union) === 1 ? $union[0] : '';
        }
        if ($candidate === '') {
            return null;
        }
        if (preg_match('/^(array|list)\s*<(.+)>$/is', $candidate, $matches) === 1) {
            $contents = trim($matches[2]);
            $parts = [];
            $depth = 0;
            $start = 0;
            for ($index = 0, $length = strlen($contents); $index < $length; ++$index) {
                $character = $contents[$index];
                if ($character === '<') {
                    ++$depth;
                } elseif ($character === '>') {
                    --$depth;
                } elseif ($character === ',' && $depth === 0) {
                    $parts[] = trim(substr($contents, $start, $index - $start));
                    $start = $index + 1;
                }
            }
            $parts[] = trim(substr($contents, $start));
            if ($matches[1] === 'array' && count($parts) === 2) {
                return ['key' => $parts[0], 'value' => $parts[1]];
            }
            return ['key' => null, 'value' => $parts[count($parts) - 1]];
        }
        if (str_ends_with($candidate, '[]')) {
            return ['key' => null, 'value' => substr($candidate, 0, -2)];
        }
        if (strtolower($candidate) === 'array') {
            return null;
        }
        return null;
    }

private function docType(?string $doc): ?string
    {
        if ($doc === null || $doc === '') {
            return null;
        }
        if (preg_match('/@phpstan-var\s+([^\r\n]+)/', $doc, $matches) === 1) {
            $rawType = trim($matches[1]);
        } elseif (preg_match('/@var\s+([^\r\n]+)/', $doc, $matches) === 1) {
            $rawType = trim($matches[1]);
        } else {
            return null;
        }
        $type = preg_replace('/\s*\*\/?\s*$/', '', $rawType) ?? $rawType;
        return $this->trimTypeDescription($type);
    }

private function trimTypeDescription(string $type): string
    {
        $depth = 0;
        for ($index = 0, $length = strlen($type); $index < $length; ++$index) {
            $character = $type[$index];
            if ($character === '<' || $character === '(' || $character === '[') {
                ++$depth;
                continue;
            }
            if ($character === '>' || $character === ')' || $character === ']') {
                --$depth;
                continue;
            }
            if (!ctype_space($character) || $depth !== 0) {
                continue;
            }
            $before = rtrim(substr($type, 0, $index));
            $after = ltrim(substr($type, $index));
            if ($after !== '' && str_starts_with($after, '|')) {
                continue;
            }
            if (str_ends_with($before, '|')) {
                continue;
            }
            return $before;
        }
        return trim($type);
    }

private function arrayElementType(?string $type, ?string $doc): ?string
    {
        $candidate = $this->docType($doc) ?? $type;
        if ($candidate === null) {
            return null;
        }
        $candidate = ltrim($candidate, '?');
        if (preg_match('/^(?:array|list)\s*<(.+)>$/is', $candidate, $matches) === 1) {
            $contents = trim($matches[1]);
            $depth = 0;
            for ($index = 0, $length = strlen($contents); $index < $length; ++$index) {
                $character = $contents[$index];
                if ($character === '<') {
                    ++$depth;
                } elseif ($character === '>') {
                    --$depth;
                } elseif ($character === ',' && $depth === 0) {
                    return trim(substr($contents, $index + 1));
                }
            }
            return $contents;
        }
        if (str_contains($candidate, '|')) {
            $parts = array_values(array_filter(array_map('trim', explode('|', $candidate)), static fn (string $part): bool => strtolower($part) !== 'null'));
            $candidate = count($parts) === 1 ? $parts[0] : null;
        }
        if ($candidate === null) {
            return null;
        }
        if (preg_match('/^(?:array|list)\s*<(.+)>$/is', $candidate, $matches) === 1) {
            $contents = trim($matches[1]);
            $depth = 0;
            for ($index = 0, $length = strlen($contents); $index < $length; ++$index) {
                $character = $contents[$index];
                if ($character === '<') {
                    ++$depth;
                } elseif ($character === '>') {
                    --$depth;
                } elseif ($character === ',' && $depth === 0) {
                    return trim(substr($contents, $index + 1));
                }
            }
            return $contents;
        }
        if (str_ends_with($candidate, '[]')) {
            return substr($candidate, 0, -2);
        }
        if (strtolower($candidate) === 'array') {
            return null;
        }
        return null;
    }
}
