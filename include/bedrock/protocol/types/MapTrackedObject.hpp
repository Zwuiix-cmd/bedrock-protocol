#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <optional>

namespace bedrock::protocol {


class MapTrackedObject {
public:
    enum class Type : uint8_t {
        Entity = 0,
        Block = 1,
        Other = 2,
    };

    Type type;
    std::optional<int64_t> actorUniqueId;
    std::optional<glm::ivec3> blockPosition;

    MapTrackedObject() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = magic_enum::enum_cast<Type>(
            stream.readUnsignedInt().value_or(0)
        ).value_or(Type::Entity);
        actorUniqueId = stream.readOptional([&]() {
            return stream.readActorUniqueID().value_or(0);
        });
        blockPosition = stream.readOptional([&]() {
            return stream.readSignedBlockPos().value_or(glm::ivec3(0));
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedInt(magic_enum::enum_integer(type));
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
            stream.writeActorUniqueID(optionalValue);
        });
        stream.writeOptional(blockPosition, [&](const auto& optionalValue) {
            stream.writeSignedBlockPos(optionalValue);
        });
    }
};

} // namespace bedrock::protocol
