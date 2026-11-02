#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/PlayerAction.hpp>

struct PlayerBlockAction {
    int32_t actionType = 0;
    glm::ivec3 blockPosition = glm::ivec3(0);
    int32_t face = 0;

    explicit PlayerBlockAction(
        const int32_t& actionTypeValue,
        const glm::ivec3& blockPositionValue,
        const int32_t& faceValue
    )
        : actionType(actionTypeValue),
          blockPosition(blockPositionValue),
          face(faceValue)
    {}

    void write(BinaryStream& stream) const {
        stream.writeVarInt(actionType);
        stream.writeSignedBlockPos(blockPosition);
        stream.writeVarInt(face);
    }

    void read(ReadOnlyBinaryStream& stream) {
        actionType = stream.readVarInt().value_or(0);
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readVarInt().value_or(0);
    }

    static bool isValidActionType(int32_t value) noexcept {
        return
            value == static_cast<int32_t>(PlayerAction::Stop::Break)
            || value == static_cast<int32_t>(PlayerAction::AbortBreak)
            || value == static_cast<int32_t>(PlayerAction::Start::Break)
            || value == static_cast<int32_t>(PlayerAction::Crack::Break)
            || value == static_cast<int32_t>(PlayerAction::PredictDestroyBlock)
            || value == static_cast<int32_t>(PlayerAction::ContinueDestroyBlock);
    }

    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, int32_t value) {
        PlayerBlockAction result;
        result.actionType = value;
        result.read(stream);
        return result;
    }
};