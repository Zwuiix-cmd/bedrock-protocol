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
                return 'stream.readOptionalVariant<' . $variant . '>([&]() { return ' . $first . '; })';
            }
            return $variant . '{' . $first . '}';
        }
        if ($this->nullableType($type)) {
            $base = $this->withoutNullType($type);
            $objectCallback = $this->readObjectCallback($base);
            if ($objectCallback !== null) {
                return 'stream.readOptional(' . $objectCallback . ')';
            }
            return 'stream.readOptional(' . $this->callback('', 'return ' . $this->fallbackReadValue($base, null, $declaration) . ';') . ')';
        }
        $arrayParts = $this->arrayTypeParts($type, $doc);
        if ($arrayParts !== null && $arrayParts['key'] !== null) {
            $mapType = $this->cppType($type, false, $doc);
            $key = $this->fallbackReadValue($arrayParts['key'], null, $declaration);
            $value = $this->fallbackReadValue($arrayParts['value'], null, $declaration);
            return 'stream.readMap<' . $mapType . '>([&]() { return ' . $key . '; }, '
                . '[&]() { return ' . $value . '; })';
        }
        $element = $this->arrayElementType($type, $doc);
        if ($element !== null) {
            if (in_array(strtolower($element), ['int8', 'uint8', 'byte', 'unsigned byte'], true)) {
                return 'stream.readByteArray().value_or(std::vector<uint8_t>())';
            }
            $objectType = $this->readObjectListType($element);
            return $objectType === null
                ? 'stream.readList(' . $this->readListCallback($element, $declaration) . ')'
                : 'stream.readObjectList<' . $objectType . '>()';
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
            return CPPName::type($type) . '::create(stream)';
        }
        $cppType = $this->cppType($type);
        if (!str_starts_with($cppType, 'std::shared_ptr<')) {
            return 'stream.readValue<' . $cppType . '>()';
        }
        return 'stream.readObject<' . CPPName::type($type) . '>()';
    }

    private function readObjectCallback(string $type): ?string
    {
        $short = CPPName::type($type);
        $referenced = $this->findDeclaration($short);
        if ($referenced !== null
            && in_array($referenced['kind'] ?? null, ['class', 'interface'], true)
            && !$this->isEnumLikeDeclaration($referenced)) {
            if (($referenced['abstract'] ?? false) || ($referenced['kind'] ?? null) === 'interface') {
            return $this->callback('', 'return ' . $short . '::create(stream);');
            }
            return $this->readObjectCallbackForCppType($this->cppType($short));
        }

        $codec = $this->catalog->typeCodec($short);
        if (is_array($codec)
            && ($codec['value'] ?? false)
            && is_string($codec['read'] ?? null)
            && preg_match('/\b[A-Za-z_]\w*\s+value;\s*value\.read\(stream\)/s', $codec['read']) === 1) {
            return $this->readObjectCallbackForCppType($this->cppType($short));
        }
        return null;
    }

    private function readObjectCallbackForCppType(string $cppType): ?string
    {
        if (preg_match('/^std::shared_ptr<([A-Za-z_]\w*)>$/', $cppType, $matches) === 1) {
            return $this->callback('', 'return stream.readObject<' . $matches[1] . '>();');
        }
        if (!preg_match('/^[A-Za-z_]\w*$/', $cppType)) {
            return null;
        }
        return $this->callback('', implode("\n", [
            $cppType . ' value;',
            'value.read(stream);',
            'return value;',
        ]));
    }

    private function readListCallback(string $type, array $declaration): string
    {
        $objectCallback = $this->readObjectCallback($type);
        if ($objectCallback !== null) {
            return $objectCallback;
        }
        return $this->callback(
            '',
            'return ' . $this->fallbackReadValue($type, null, $declaration) . ';',
        );
    }

    private function readObjectListType(string $type): ?string
    {
        $cppType = $this->cppType($type);
        $referenced = $this->findDeclaration(CPPName::type($type));
        if ($referenced !== null
            && (($referenced['abstract'] ?? false) || ($referenced['kind'] ?? null) === 'interface')) {
            return null;
        }
        return preg_match('/^std::shared_ptr<([A-Za-z_]\w*)>$/', $cppType, $matches) === 1
            && $this->readObjectCallback($type) !== null
            ? $matches[1]
            : null;
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
            return 'stream.writeByte(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['uint8', 'uint8_t', 'unsigned byte'], true)) {
            return 'stream.writeUnsignedByte(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['int16', 'int16_t', 'short'], true)) {
            return 'stream.writeShort(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['uint16', 'uint16_t', 'unsigned short'], true)) {
            return 'stream.writeUnsignedShort(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['int', 'int32', 'int32_t', 'signed int'], true)) {
            return 'stream.writeInt(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['uint32', 'uint32_t', 'unsigned int'], true)) {
            return 'stream.writeUnsignedInt(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['int64', 'int64_t', 'long', 'long long'], true)) {
            return 'stream.writeLong(' . $this->enumStorageValueForWrite($value) . ');';
        }
        if (in_array($normalized, ['uint64', 'uint64_t', 'unsigned long', 'unsigned long long'], true)) {
            return 'stream.writeUnsignedLong(' . $this->enumStorageValueForWrite($value) . ');';
        }
        $short = CPPName::type($type);
        $codec = $this->catalog->typeCodec($short);
        if (is_array($codec) && isset($codec['write']) && is_string($codec['write'])) {
            return str_replace('%VALUE%', $value, $codec['write']);
        }
        $referenced = $this->findDeclaration($short);
        if ($referenced !== null && $this->isEnumLikeDeclaration($referenced)) {
            return str_contains($value, 'magic_enum::enum_integer')
                ? 'stream.writeVarInt(' . $value . ');'
                : 'stream.writeVarInt(magic_enum::enum_integer(' . $value . '));';
        }
        if ($referenced !== null && in_array($referenced['kind'], ['class', 'interface'], true)) {
            $cppType = $this->cppType($short);
            if (str_starts_with($cppType, 'std::shared_ptr<')) {
                return $this->nullableType($type)
                    ? 'if (' . $value . ') ' . $value . '->write(stream);'
                    : $value . '->write(stream);';
            }
            return $value . '.write(stream);';
        }
        return '(void)(' . $value . ');';
    }

    private function enumStorageValueForWrite(string $value): string
    {
        if (preg_match('/^[A-Za-z_]\w*$/', $value) !== 1) {
            return $value;
        }
        $type = $this->expressionTypes[$value] ?? null;
        if ($type === null) {
            return $value;
        }
        $referenced = $this->findDeclaration(CPPName::type($type));
        return $this->isEnumLikeDeclaration($referenced ?? [])
            ? 'magic_enum::enum_integer(' . $value . ')'
            : $value;
    }
}
