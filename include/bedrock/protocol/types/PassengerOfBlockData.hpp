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
#include <bedrock/protocol/types/EmoteType.hpp>
#include <bedrock/protocol/types/EmoteType.hpp>
#include <bedrock/protocol/types/EmoteType.hpp>

class PassengerOfBlockData {
public:
    glm::ivec3 blockPos;
    glm::vec3 offset;
    float rotation;
    float rotationLimit;
    EmoteType emoteType;

    PassengerOfBlockData() = default;
    explicit PassengerOfBlockData(
        const glm::ivec3& blockPosValue,
        const glm::vec3& offsetValue,
        const float& rotationValue,
        const float& rotationLimitValue,
        const EmoteType& emoteTypeValue
    ) :
        blockPos(blockPosValue),
        offset(offsetValue),
        rotation(rotationValue),
        rotationLimit(rotationLimitValue),
        emoteType(emoteTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        blockPos = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        offset = stream.readVec3().value_or(glm::vec3(0.0f));
        rotation = stream.readFloat().value_or(0.0f);
        rotationLimit = stream.readFloat().value_or(0.0f);
        emoteType = static_cast<EmoteType>(stream.readVarInt().value_or(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeSignedBlockPos(blockPos);
		stream.writeVec3(offset);
		stream.writeFloat(rotation);
		stream.writeFloat(rotationLimit);
		stream.writeUnsignedByte(magic_enum::enum_integer(emoteType));
    }
};
