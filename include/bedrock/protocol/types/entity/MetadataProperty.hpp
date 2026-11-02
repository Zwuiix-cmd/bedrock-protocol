#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <map>
#include <memory>
#include <string>
#include <utility>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/NetworkNbt.hpp>

namespace bedrock::protocol {


enum class EntityMetadataType : uint32_t {
    BYTE = 0,
    SHORT = 1,
    INT = 2,
    FLOAT = 3,
    STRING = 4,
    COMPOUND_TAG = 5,
    POS = 6,
    LONG = 7,
    VECTOR3F = 8,
};

class MetadataProperty {
public:
    virtual ~MetadataProperty() = default;
    [[nodiscard]] virtual EntityMetadataType getType() const noexcept = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;
    virtual void write(BinaryStream& stream) const = 0;
};

class ByteMetadataProperty : public MetadataProperty {
public:
    using value_type = int8_t;
    value_type value = 0;

    ByteMetadataProperty() = default;
    explicit ByteMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::BYTE; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readByte().value_or(0); }
    void write(BinaryStream& stream) const override { stream.writeByte(value); }
};

class ShortMetadataProperty : public MetadataProperty {
public:
    using value_type = int16_t;
    value_type value = 0;

    ShortMetadataProperty() = default;
    explicit ShortMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::SHORT; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readShort().value_or(0); }
    void write(BinaryStream& stream) const override { stream.writeShort(value); }
};

class IntMetadataProperty : public MetadataProperty {
public:
    using value_type = int32_t;
    value_type value = 0;

    IntMetadataProperty() = default;
    explicit IntMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::INT; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readVarInt().value_or(0); }
    void write(BinaryStream& stream) const override { stream.writeVarInt(value); }
};

class FloatMetadataProperty : public MetadataProperty {
public:
    using value_type = float;
    value_type value = 0.0f;

    FloatMetadataProperty() = default;
    explicit FloatMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::FLOAT; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readFloat().value_or(0.0f); }
    void write(BinaryStream& stream) const override { stream.writeFloat(value); }
};

class StringMetadataProperty : public MetadataProperty {
public:
    using value_type = std::string;
    value_type value = "";

    StringMetadataProperty() = default;
    explicit StringMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::STRING; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readString().value_or(""); }
    void write(BinaryStream& stream) const override { stream.writeString(value); }
};

class CompoundTagMetadataProperty : public MetadataProperty {
public:
    using value_type = std::shared_ptr<CompoundTag>;
    value_type value;

    CompoundTagMetadataProperty() = default;
    explicit CompoundTagMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::COMPOUND_TAG; }
    void read(ReadOnlyBinaryStream& stream) override { value = NetworkNbtSerializer::read(stream); }
    void write(BinaryStream& stream) const override { NetworkNbtSerializer::write(stream, value); }
};

class BlockPosMetadataProperty : public MetadataProperty {
public:
    using value_type = glm::ivec3;
    value_type value = glm::ivec3(0);

    BlockPosMetadataProperty() = default;
    explicit BlockPosMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::POS; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readSignedBlockPos().value_or(glm::ivec3(0)); }
    void write(BinaryStream& stream) const override { stream.writeSignedBlockPos(value); }
};

class LongMetadataProperty : public MetadataProperty {
public:
    using value_type = int64_t;
    value_type value = 0;

    LongMetadataProperty() = default;
    explicit LongMetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::LONG; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readVarLong().value_or(0); }
    void write(BinaryStream& stream) const override { stream.writeVarLong(value); }
};

class Vec3MetadataProperty : public MetadataProperty {
public:
    using value_type = glm::vec3;
    value_type value = glm::vec3(0.0f);

    Vec3MetadataProperty() = default;
    explicit Vec3MetadataProperty(value_type input) : value(std::move(input)) {}

    [[nodiscard]] EntityMetadataType getType() const noexcept override { return EntityMetadataType::VECTOR3F; }
    void read(ReadOnlyBinaryStream& stream) override { value = stream.readVec3().value_or(glm::vec3(0.0f)); }
    void write(BinaryStream& stream) const override { stream.writeVec3(value); }
};

class EntityMetadataCollection {
public:
    std::map<uint32_t, std::unique_ptr<MetadataProperty>> properties;

    void read(ReadOnlyBinaryStream& stream) {
        const uint32_t count = stream.readUnsignedVarInt().value_or(0);
        const bool countAvailable = stream.canReadCollection(count);
        for (uint32_t index = 0; index < count && countAvailable && !stream.hasOverflowed(); ++index) {
            const uint32_t key = stream.readUnsignedVarInt().value_or(0);
            if (properties.contains(key)) {
                stream.seek(stream.size());
                return;
            }
            const auto type = magic_enum::enum_cast<EntityMetadataType>(stream.readUnsignedVarInt().value_or(0)).value_or(EntityMetadataType::BYTE);
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
            stream.writeUnsignedVarInt(magic_enum::enum_integer(type));
            stream.writeUnsignedByte(magic_enum::enum_integer(type));
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
    static std::unique_ptr<MetadataProperty> create(EntityMetadataType type) {        if (type == EntityMetadataType::BYTE) return std::make_unique<ByteMetadataProperty>();
        if (type == EntityMetadataType::SHORT) return std::make_unique<ShortMetadataProperty>();
        if (type == EntityMetadataType::INT) return std::make_unique<IntMetadataProperty>();
        if (type == EntityMetadataType::FLOAT) return std::make_unique<FloatMetadataProperty>();
        if (type == EntityMetadataType::STRING) return std::make_unique<StringMetadataProperty>();
        if (type == EntityMetadataType::COMPOUND_TAG) return std::make_unique<CompoundTagMetadataProperty>();
        if (type == EntityMetadataType::POS) return std::make_unique<BlockPosMetadataProperty>();
        if (type == EntityMetadataType::LONG) return std::make_unique<LongMetadataProperty>();
        if (type == EntityMetadataType::VECTOR3F) return std::make_unique<Vec3MetadataProperty>();
        return nullptr;
    }
};

} // namespace bedrock::protocol
