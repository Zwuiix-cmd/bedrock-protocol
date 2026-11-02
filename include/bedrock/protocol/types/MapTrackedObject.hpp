#pragma once

#include <cstdint>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/Forward.hpp>

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
        type = static_cast<Type>(stream.readUnsignedInt().value_or(0));
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        actorUniqueId = stream.readActorUniqueID().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedInt(static_cast<std::underlying_type_t<Type>>(type));
        stream.writeSignedBlockPos(blockPosition);
        stream.writeActorUniqueID(actorUniqueId);
    }
};
