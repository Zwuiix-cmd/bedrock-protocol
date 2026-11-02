<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\translation;

use PhpParser\Node;
use kodiak\bedrockprotocol\generator\CPPName;

trait ValueCodecTranslationMethods
{
private function fallbackReadValue(?string $type, ?string $doc, array $declaration): string
    {
        $type = $this->docType($doc) ?? $type ?? 'mixed';
        $unionParts = $this->unionParts($type);
        if (count($unionParts) > 1) {
            $variant = $this->cppType($type);
            $first = $this->fallbackReadValue($unionParts[0], null, $declaration);
            if ($this->nullableType($type)) {
                return '([&]() { auto value = stream.readOptional([&]() { return ' . $first . '; }); return value ? ' . $variant . '{*value} : ' . $variant . '{std::monostate{}}; }())';
            }
            return $variant . '{' . $first . '}';
        }
        if ($this->nullableType($type)) {
            $base = $this->withoutNullType($type);
            return 'stream.readOptional(' . $this->callback('', 'return ' . $this->fallbackReadValue($base, null, $declaration) . ';') . ')';
        }
        $arrayParts = $this->arrayTypeParts($type, $doc);
        if ($arrayParts !== null && $arrayParts['key'] !== null) {
            $mapType = $this->cppType($type, false, $doc);
            $key = $this->fallbackReadValue($arrayParts['key'], null, $declaration);
            $value = $this->fallbackReadValue($arrayParts['value'], null, $declaration);
            return "([&]() {\n"
                . '    ' . $mapType . " result;\n"
                . "    const auto count = stream.readUnsignedInt().value_or(0);\n"
                . "    for (uint32_t i = 0; i < count; ++i) {\n"
                . '        auto key = ' . $key . ";\n"
                . '        auto value = ' . $value . ";\n"
                . "        result.emplace(std::move(key), std::move(value));\n"
                . "    }\n"
                . "    return result;\n"
                . "}())";
        }
        $element = $this->arrayElementType($type, $doc);
        if ($element !== null) {
            if (in_array(strtolower($element), ['int8', 'uint8', 'byte', 'unsigned byte'], true)) {
                return 'stream.readByteArray().value_or(std::vector<uint8_t>())';
            }
            return 'stream.readList(' . $this->callback('', 'return ' . $this->fallbackReadValue($element, null, $declaration) . ';') . ')';
        }
        $normalized = strtolower(ltrim($type, '\\'));
        if ($normalized === 'string' || $normalized === 'stringview') {
            return 'stream.readString().value_or("")';
        }
        if ($normalized === 'bool') {
            return 'stream.readBool().value_or(false)';
        }
        if ($normalized === 'float') {
            return 'stream.readFloat().value_or(0.0f)';
        }
        if ($normalized === 'double') {
            return 'stream.readDouble().value_or(0.0)';
        }
        if (in_array($normalized, ['int8', 'int8_t', 'byte'], true)) {
            return 'stream.readByte().value_or(0)';
        }
        if (in_array($normalized, ['uint8', 'uint8_t', 'unsigned byte'], true)) {
            return 'stream.readUnsignedByte().value_or(0)';
        }
        if (in_array($normalized, ['int16', 'int16_t', 'short'], true)) {
            return 'stream.readShort().value_or(0)';
        }
        if (in_array($normalized, ['uint16', 'uint16_t', 'unsigned short'], true)) {
            return 'stream.readUnsignedShort().value_or(0)';
        }
        if (in_array($normalized, ['int', 'int32', 'int32_t', 'signed int'], true)) {
            return 'stream.readInt().value_or(0)';
        }
        if (in_array($normalized, ['uint32', 'uint32_t', 'unsigned int'], true)) {
            return 'stream.readUnsignedInt().value_or(0)';
        }
        if (in_array($normalized, ['int64', 'int64_t', 'long', 'long long'], true)) {
            return 'stream.readLong().value_or(0)';
        }
        if (in_array($normalized, ['uint64', 'uint64_t', 'unsigned long', 'unsigned long long'], true)) {
            return 'stream.readUnsignedLong().value_or(0)';
        }
        $short = CPPName::type($type);
        $codec = $this->catalog->typeCodec($short);
        if (is_array($codec) && isset($codec['read']) && is_string($codec['read'])) {
            return $codec['read'];
        }
        $referenced = $this->findDeclaration($short);
        if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
            return $this->enumReadValue($short, 'stream.readVarInt().value_or(0)', $declaration);
        }
        if ($referenced !== null && in_array($referenced['kind'], ['class', 'interface'], true)) {
            return $this->readObject($short);
        }
        return 'std::monostate{}';
    }

private function readObject(string $type): string
    {
        $declaration = $this->findDeclaration(CPPName::type($type));
        if ($declaration !== null
            && (($declaration['abstract'] ?? false) || ($declaration['kind'] ?? null) === 'interface')) {
            return CPPName::type($type) . '::decode(stream)';
        }
        $cppType = $this->cppType($type);
        if (!str_starts_with($cppType, 'std::shared_ptr<')) {
            return "([&]() {\n"
                . '    ' . $cppType . " value;\n"
                . "    value.read(stream);\n"
                . "    return value;\n"
                . "}())";
        }
        return "([&]() {\n"
            . '    auto value = std::make_shared<' . CPPName::type($type) . ">();\n"
            . "    value->read(stream);\n"
            . "    return value;\n"
            . "}())";
    }

private function constructorArguments(Node\Expr\New_ $expression, string $className, array $declaration, ?array $method): string
    {
        $referenced = $this->findDeclaration($className);
        $properties = array_values(array_filter($referenced['properties'] ?? [], static fn (array $property): bool => !($property['static'] ?? false)));
        $arguments = [];
        foreach ($expression->args as $index => $argument) {
            if (!$argument instanceof Node\Arg) {
                $arguments[] = $this->argumentExpression($argument, $declaration, $method);
                continue;
            }
            $expected = isset($properties[$index])
                ? $this->cppPropertyType($properties[$index], $referenced)
                : null;
            $value = $this->expression($argument->value, $declaration, $method, $expected);
            if ($this->isOptionalProperty($argument->value, $declaration) || $this->isOptionalExpression($argument->value)) {
                $value = '*(' . $value . ')';
            }
            $arguments[] = $value;
        }
        return implode(', ', $arguments);
    }

private function fallbackWriteValue(?string $type, ?string $doc, string $value, array $declaration): string
    {
        $type = $this->docType($doc) ?? $type ?? 'mixed';
        $unionParts = $this->unionParts($type);
        if (count($unionParts) > 1) {
            $branches = [];
            foreach ($unionParts as $part) {
                $alternative = $this->cppType($part);
                $write = $this->fallbackWriteValue($part, null, 'alternative', $declaration);
                $branches[] = 'if constexpr (std::is_same_v<Alternative, ' . $alternative . '>) { ' . $write . ' }';
            }
            return 'std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; ' . implode(' else ', $branches) . ' }, ' . $value . ');';
        }
        if ($this->nullableType($type)) {
            $base = $this->withoutNullType($type);
            return 'stream.writeOptional(' . $value . ', ' . $this->callback('const auto& optionalValue', $this->fallbackWriteValue($base, null, 'optionalValue', $declaration)) . ');';
        }
        $arrayParts = $this->arrayTypeParts($type, $doc);
        if ($arrayParts !== null && $arrayParts['key'] !== null) {
            $key = $this->fallbackWriteValue($arrayParts['key'], null, 'key', $declaration);
            $item = $this->fallbackWriteValue($arrayParts['value'], null, 'item', $declaration);
            return 'stream.writeUnsignedInt(' . $value . ".size());\n"
                . 'for (const auto& [key, item] : ' . $value . ") {\n"
                . '    ' . $key . "\n"
                . '    ' . $item . "\n"
                . "}";
        }
        $element = $this->arrayElementType($type, $doc);
        if ($element !== null) {
            if (in_array(strtolower($element), ['int8', 'uint8', 'byte', 'unsigned byte'], true)) {
                return 'stream.writeByteArray(' . $value . ');';
            }
            return 'stream.writeList(' . $value . ', ' . $this->callback('const auto& item', $this->fallbackWriteValue($element, null, 'item', $declaration)) . ');';
        }
        $normalized = strtolower(ltrim($type, '\\'));
        if ($normalized === 'string' || $normalized === 'stringview') {
            return 'stream.writeString(' . $value . ');';
        }
        if ($normalized === 'bool') {
            return 'stream.writeBool(' . $value . ');';
        }
        if ($normalized === 'float') {
            return 'stream.writeFloat(' . $value . ');';
        }
        if ($normalized === 'double') {
            return 'stream.writeDouble(' . $value . ');';
        }
        if (in_array($normalized, ['int8', 'int8_t', 'byte'], true)) {
            return 'stream.writeByte(' . $value . ');';
        }
        if (in_array($normalized, ['uint8', 'uint8_t', 'unsigned byte'], true)) {
            return 'stream.writeUnsignedByte(' . $value . ');';
        }
        if (in_array($normalized, ['int16', 'int16_t', 'short'], true)) {
            return 'stream.writeShort(' . $value . ');';
        }
        if (in_array($normalized, ['uint16', 'uint16_t', 'unsigned short'], true)) {
            return 'stream.writeUnsignedShort(' . $value . ');';
        }
        if (in_array($normalized, ['int', 'int32', 'int32_t', 'signed int'], true)) {
            return 'stream.writeInt(' . $value . ');';
        }
        if (in_array($normalized, ['uint32', 'uint32_t', 'unsigned int'], true)) {
            return 'stream.writeUnsignedInt(' . $value . ');';
        }
        if (in_array($normalized, ['int64', 'int64_t', 'long', 'long long'], true)) {
            return 'stream.writeLong(' . $value . ');';
        }
        if (in_array($normalized, ['uint64', 'uint64_t', 'unsigned long', 'unsigned long long'], true)) {
            return 'stream.writeUnsignedLong(' . $value . ');';
        }
        $short = CPPName::type($type);
        $codec = $this->catalog->typeCodec($short);
        if (is_array($codec) && isset($codec['write']) && is_string($codec['write'])) {
            return str_replace('%VALUE%', $value, $codec['write']);
        }
        $referenced = $this->findDeclaration($short);
        if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
            return 'stream.writeVarInt(magic_enum::enum_integer(' . $value . '));';
        }
        if ($referenced !== null && in_array($referenced['kind'], ['class', 'interface'], true)) {
            return 'if (' . $value . ') ' . $value . '->write(stream);';
        }
        return '(void)(' . $value . ');';
    }
}
