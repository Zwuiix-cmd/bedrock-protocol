#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>


class MapTrackedObject {
public:
    enum class Type : uint8_t {
        Entity = 0,
        Block = 1,
    };

    Type type;
    int64_t actorUniqueId;
    glm::ivec3 blockPosition;

    MapTrackedObject() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = magic_enum::enum_cast<Type>(
            stream.readUnsignedInt().value_or(0)
        ).value_or(Type::Entity);
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        actorUniqueId = stream.readActorUniqueID().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(magic_enum::enum_integer(type));
        stream.writeSignedBlockPos(blockPosition);
        stream.writeActorUniqueID(actorUniqueId);
    }
};
