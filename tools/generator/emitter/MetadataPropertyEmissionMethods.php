<?php

declare(strict_types=1);

namespace kodiak\bedrockprotocol\generator\emitter;

use kodiak\bedrockprotocol\generator\Configuration;
use kodiak\bedrockprotocol\generator\CPPName;
use kodiak\bedrockprotocol\generator\CPP\CPPDefaultValueResolver;
use RuntimeException;

trait MetadataPropertyEmissionMethods
{
    private function metadataPropertyHeader(array $declarations): string
    {
        $typeDeclaration = $declarations['EntityMetadataTypes'] ?? null;
        $metadataTypes = [];
        foreach (($typeDeclaration['constants'] ?? []) as $constant) {
            if (is_int($constant['value'] ?? null)) {
                $metadataTypes[(int) $constant['value']] = CPPName::identifier((string) $constant['name']);
            }
        }

        $properties = [];
        foreach ($declarations as $declaration) {
            $name = (string) ($declaration['name'] ?? '');
            if (!str_ends_with($name, 'MetadataProperty') || $name === 'MetadataProperty') {
                continue;
            }
            $id = $this->constantInteger($declaration, 'ID');
            if ($id === null || !isset($metadataTypes[$id])) {
                continue;
            }
            $properties[] = [
                'declaration' => $declaration,
                'id' => $id,
                'enum' => $metadataTypes[$id],
            ];
        }
        usort($properties, static fn (array $left, array $right): int => $left['id'] <=> $right['id']);

        $body = <<<'CPP'
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/NetworkNbt.hpp>

enum class EntityMetadataType : uint32_t {
CPP;
        $body .= "\n";
        foreach ($metadataTypes as $value => $name) {
            $body .= '    ' . $name . ' = ' . $value . ",\n";
        }
        $body .= "};\n\nclass MetadataProperty {\npublic:\n    virtual ~MetadataProperty() = default;\n    [[nodiscard]] virtual EntityMetadataType getType() const noexcept = 0;\n    virtual void read(ReadOnlyBinaryStream& stream) = 0;\n    virtual void write(BinaryStream& stream) const = 0;\n};\n\n";

        foreach ($properties as $property) {
            $declaration = $property['declaration'];
            $name = CPPName::type((string) $declaration['name']);
            $codec = $this->metadataCodec($name);
            $body .= 'class ' . $name . " : public MetadataProperty {\npublic:\n";
            $body .= '    using value_type = ' . $codec['type'] . ";\n";
            $body .= '    value_type value';
            $default = CPPDefaultValueResolver::member($codec['type']);
            $body .= $default === null ? ";\n\n" : ' = ' . $default . ";\n\n";
            $body .= '    ' . $name . '() = default;' . "\n";
            $body .= '    explicit ' . $name . '(value_type input) : value(std::move(input)) {}' . "\n\n";
            $body .= '    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::' . $property['enum'] . "; }\n";
            $body .= '    void read(ReadOnlyBinaryStream& stream) override { value = ' . $codec['read'] . "; }\n";
            $body .= '    void write(BinaryStream& stream) const override { ' . $codec['write'] . "; }\n";
            $body .= "};\n\n";
        }

        $body .= <<<'CPP'
class EntityMetadataCollection {
public:
    std::map<uint32_t, std::unique_ptr<MetadataProperty>> properties;

    void read(ReadOnlyBinaryStream& stream) {
        const uint32_t count = stream.readUnsignedVarInt().value_or(0);
        for (uint32_t index = 0; index < count; ++index) {
            const uint32_t key = stream.readUnsignedVarInt().value_or(0);
            if (properties.contains(key)) {
                stream.seek(stream.size());
                return;
            }
            const auto type = static_cast<EntityMetadataType>(stream.readUnsignedVarInt().value_or(0));
            const uint8_t innerType = stream.readUnsignedByte().value_or(0);
            if (innerType != static_cast<uint8_t>(type)) {
                stream.seek(stream.size());
                return;
            }
            auto property = create(type);
            if (!property) {
                stream.seek(stream.size());
                return;
            }
            property->read(stream);
            properties.emplace(key, std::move(property));
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(static_cast<uint32_t>(properties.size()));
        for (const auto& [key, property] : properties) {
            if (!property) {
                continue;
            }
            const auto type = property->getType();
            stream.writeUnsignedVarInt(key);
            stream.writeUnsignedVarInt(static_cast<uint32_t>(type));
            stream.writeUnsignedByte(static_cast<uint8_t>(type));
            property->write(stream);
        }
    }

    template <typename Property, typename... Args>
    Property& emplace(uint32_t key, Args&&... args) {
        auto property = std::make_unique<Property>(std::forward<Args>(args)...);
        auto* result = property.get();
        properties[key] = std::move(property);
        return *result;
    }

private:
    static std::unique_ptr<MetadataProperty> create(EntityMetadataType type) {
CPP;
        foreach ($properties as $property) {
            $name = CPPName::type((string) $property['declaration']['name']);
            $body .= '        if (type == EntityMetadataType::' . $property['enum'] . ') return std::make_unique<' . $name . '>();' . "\n";
        }
        $body .= <<<'CPP'
        return nullptr;
    }
};
CPP;
        return $body;
    }

    private function runtimePropertyType(array $property): string
    {
        $name = strtolower((string) ($property['name'] ?? ''));
        if (str_contains($name, 'actoruniqueid')) {
            return 'int64_t';
        }
        return $this->translator->cppType($property['type'] ?? null, false, $property['doc'] ?? null);
    }

    private function runtimeReadExpression(array $property, bool $varInt = false): string
    {
        $name = strtolower((string) ($property['name'] ?? ''));
        $type = ltrim((string) ($property['type'] ?? ''), '?');
        if (CPPName::type($type) === 'Vector2') {
            return 'stream.readVec2().value_or(glm::vec2(0.0f))';
        }
        if (CPPName::type($type) === 'Vector3') {
            return 'stream.readVec3().value_or(glm::vec3(0.0f))';
        }
        if (CPPName::type($type) === 'BlockPosition') {
            return 'stream.readSignedBlockPos().value_or(glm::ivec3(0))';
        }
        if (str_contains($name, 'actoruniqueid')) {
            return 'stream.readActorUniqueID().value_or(0)';
        }
        if ($type === 'int') {
            return ($varInt ? 'stream.readVarInt()' : 'stream.readInt()') . '.value_or(0)';
        }
        if ($type === 'float') {
            return 'stream.readFloat().value_or(0.0f)';
        }
        if ($type === 'bool') {
            return 'stream.readBool().value_or(false)';
        }
        if ($type === 'string') {
            return 'stream.readString().value_or("")';
        }
        return 'std::monostate{}';
    }

    private function runtimeWriteExpression(array $property, string $value, bool $varInt = false): string
    {
        $name = strtolower((string) ($property['name'] ?? ''));
        $type = ltrim((string) ($property['type'] ?? ''), '?');
        if (CPPName::type($type) === 'Vector2') {
            return 'stream.writeVec2(' . $value . ');';
        }
        if (CPPName::type($type) === 'Vector3') {
            return 'stream.writeVec3(' . $value . ');';
        }
        if (CPPName::type($type) === 'BlockPosition') {
            return 'stream.writeSignedBlockPos(' . $value . ');';
        }
        if (str_contains($name, 'actoruniqueid')) {
            return 'stream.writeActorUniqueID(' . $value . ');';
        }
        if ($type === 'int') {
            return 'stream.write' . ($varInt ? 'VarInt' : 'Int') . '(' . $value . ');';
        }
        if ($type === 'float') {
            return 'stream.writeFloat(' . $value . ');';
        }
        if ($type === 'bool') {
            return 'stream.writeBool(' . $value . ');';
        }
        if ($type === 'string') {
            return 'stream.writeString(' . $value . ');';
        }
        return '(void)(' . $value . ');';
    }

    private function metadataCodec(string $name): array
    {
        return match (str_replace('MetadataProperty', '', $name)) {
            'Byte' => ['type' => 'int8_t', 'read' => 'stream.readByte().value_or(0)', 'write' => 'stream.writeByte(value)'],
            'Short' => ['type' => 'int16_t', 'read' => 'stream.readShort().value_or(0)', 'write' => 'stream.writeShort(value)'],
            'Int' => ['type' => 'int32_t', 'read' => 'stream.readVarInt().value_or(0)', 'write' => 'stream.writeVarInt(value)'],
            'Float' => ['type' => 'float', 'read' => 'stream.readFloat().value_or(0.0f)', 'write' => 'stream.writeFloat(value)'],
            'String' => ['type' => 'std::string', 'read' => 'stream.readString().value_or("")', 'write' => 'stream.writeString(value)'],
            'CompoundTag' => ['type' => 'std::shared_ptr<CompoundTag>', 'read' => 'NetworkNbtSerializer::read(stream)', 'write' => 'NetworkNbtSerializer::write(stream, value)'],
            'BlockPos' => ['type' => 'glm::ivec3', 'read' => 'stream.readSignedBlockPos().value_or(glm::ivec3(0))', 'write' => 'stream.writeSignedBlockPos(value)'],
            'Long' => ['type' => 'int64_t', 'read' => 'stream.readVarLong().value_or(0)', 'write' => 'stream.writeVarLong(value)'],
            'Vec3' => ['type' => 'glm::vec3', 'read' => 'stream.readVec3().value_or(glm::vec3(0.0f))', 'write' => 'stream.writeVec3(value)'],
            default => throw new RuntimeException('Unsupported metadata property: ' . $name),
        };
    }

    private function constantInteger(array $declaration, string $name): ?int
    {
        foreach ($declaration['constants'] ?? [] as $constant) {
            if (($constant['name'] ?? null) === $name && is_int($constant['value'] ?? null)) {
                return (int) $constant['value'];
            }
        }
        return null;
    }
}
