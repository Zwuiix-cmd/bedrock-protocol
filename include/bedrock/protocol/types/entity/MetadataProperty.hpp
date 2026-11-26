#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <variant>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/NetworkNbt.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
#include <bedrock/protocol/util/ProtocolUtil.hpp>

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

using EntityMetadataValue = std::variant<
    int8_t,
    int16_t,
    int32_t,
    float,
    std::string,
    std::shared_ptr<CompoundTag>,
    glm::ivec3,
    int64_t,
    glm::vec3
>;

class EntityMetadataCollection {
public:
    std::map<uint32_t, EntityMetadataValue> properties;

    [[nodiscard]] static constexpr EntityMetadataType getType(const EntityMetadataValue& value) noexcept {
        return util::fromUnderlying<EntityMetadataType>(value.index());
    }

    template <typename T>
    [[nodiscard]] const T* get(uint32_t key) const noexcept {
        const auto it = properties.find(key);
        if (it == properties.end()) {
            return nullptr;
        }
        return std::get_if<T>(&it->second);
    }

    template <typename T>
    [[nodiscard]] const T* get(EntityMetadataProperties key) const noexcept {
        return get<T>(util::toUnderlying(key));
    }

    template <typename T>
    void set(uint32_t key, T value) {
        properties.insert_or_assign(key, EntityMetadataValue(std::move(value)));
    }

    template <typename T>
    void set(EntityMetadataProperties key, T value) {
        set<T>(util::toUnderlying(key), std::move(value));
    }

    [[nodiscard]] bool getFlag(EntityMetadataFlags flag) const noexcept {
        const auto idx = util::toUnderlying(flag);
        const auto prop = idx < 64 ? EntityMetadataProperties::Flags : EntityMetadataProperties::Flags2;
        const auto bit = idx < 64 ? idx : (idx - 64);
        const auto* val = get<int64_t>(prop);
        return val != nullptr && ((*val >> bit) & 1) != 0;
    }

    void setFlag(EntityMetadataFlags flag, bool enabled) {
        const auto idx = util::toUnderlying(flag);
        const auto prop = idx < 64 ? EntityMetadataProperties::Flags : EntityMetadataProperties::Flags2;
        const auto bit = idx < 64 ? idx : (idx - 64);
        const auto* existing = get<int64_t>(prop);
        int64_t val = existing ? *existing : 0;
        if (enabled) {
            val |= (int64_t{1} << bit);
        } else {
            val &= ~(int64_t{1} << bit);
        }
        set<int64_t>(prop, val);
    }

    void read(ReadOnlyBinaryStream& stream) {
        const uint32_t count = stream.readUnsignedVarInt().value_or(0);
        if (!stream.canReadCollection(count)) {
            stream.markOverflowed();
            return;
        }
        for (uint32_t index = 0; index < count && !stream.hasOverflowed(); ++index) {
            const uint32_t key = stream.readUnsignedVarInt().value_or(0);
            if (properties.contains(key)) {
                stream.markOverflowed();
                return;
            }
            const uint32_t rawType = stream.readUnsignedVarInt().value_or(0);
            const uint8_t innerType = stream.readUnsignedByte().value_or(0);
            if (rawType > 8 || innerType != util::narrow<uint8_t>(rawType)) {
                stream.markOverflowed();
                return;
            }
            switch (util::fromUnderlying<EntityMetadataType>(rawType)) {
                case EntityMetadataType::BYTE:
                    properties.emplace(key, stream.readByte().value_or(0));
                    break;
                case EntityMetadataType::SHORT:
                    properties.emplace(key, stream.readShort().value_or(0));
                    break;
                case EntityMetadataType::INT:
                    properties.emplace(key, stream.readVarInt().value_or(0));
                    break;
                case EntityMetadataType::FLOAT:
                    properties.emplace(key, stream.readFloat().value_or(0.0f));
                    break;
                case EntityMetadataType::STRING:
                    properties.emplace(key, stream.readString().value_or(""));
                    break;
                case EntityMetadataType::COMPOUND_TAG:
                    properties.emplace(key, NetworkNbtSerializer::read(stream));
                    break;
                case EntityMetadataType::POS:
                    properties.emplace(key, stream.readSignedBlockPos().value_or(glm::ivec3(0)));
                    break;
                case EntityMetadataType::LONG:
                    properties.emplace(key, stream.readVarLong().value_or(0));
                    break;
                case EntityMetadataType::VECTOR3F:
                    properties.emplace(key, stream.readVec3().value_or(glm::vec3(0.0f)));
                    break;
            }
        }
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(util::narrow<uint32_t>(properties.size()));
        for (const auto& [key, value] : properties) {
            const auto type = util::narrow<uint32_t>(value.index());
            stream.writeUnsignedVarInt(key);
            stream.writeUnsignedVarInt(type);
            stream.writeUnsignedByte(util::narrow<uint8_t>(type));
            std::visit([&](const auto& v) {
                using T = std::decay_t<decltype(v)>;
                if constexpr (std::is_same_v<T, int8_t>) {
                    stream.writeByte(v);
                } else if constexpr (std::is_same_v<T, int16_t>) {
                    stream.writeShort(v);
                } else if constexpr (std::is_same_v<T, int32_t>) {
                    stream.writeVarInt(v);
                } else if constexpr (std::is_same_v<T, float>) {
                    stream.writeFloat(v);
                } else if constexpr (std::is_same_v<T, std::string>) {
                    stream.writeString(v);
                } else if constexpr (std::is_same_v<T, std::shared_ptr<CompoundTag>>) {
                    NetworkNbtSerializer::write(stream, v);
                } else if constexpr (std::is_same_v<T, glm::ivec3>) {
                    stream.writeSignedBlockPos(v);
                } else if constexpr (std::is_same_v<T, int64_t>) {
                    stream.writeVarLong(v);
                } else if constexpr (std::is_same_v<T, glm::vec3>) {
                    stream.writeVec3(v);
                }
            }, value);
        }
    }
};

} // namespace bedrock::protocol
