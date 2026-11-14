#pragma once

#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/types/PlayerAction.hpp>

namespace bedrock::protocol {


struct PlayerBlockAction {
    PlayerAction actionType = PlayerAction::StopBreak;
    glm::ivec3 blockPosition = glm::ivec3(0);
    int32_t face = 0;

    PlayerBlockAction() = default;

    explicit PlayerBlockAction(
        const PlayerAction& actionTypeValue,
        const glm::ivec3& blockPositionValue,
        const int32_t& faceValue
    )
        : actionType(actionTypeValue),
          blockPosition(blockPositionValue),
          face(faceValue)
    {}

    void write(BinaryStream& stream) const {
        stream.writeVarInt(static_cast<int32_t>(actionType));
        stream.writeSignedBlockPos(blockPosition);
        stream.writeVarInt(face);
    }

    void read(ReadOnlyBinaryStream& stream) {
        actionType = static_cast<PlayerAction>(stream.readVarInt().value_or(0));
        if (!isValidActionType(actionType)) {
            stream.markOverflowed();
            return;
        }
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readVarInt().value_or(0);
    }

    static bool isValidActionType(int32_t value) noexcept {
        return
            value == static_cast<int32_t>(PlayerAction::StopBreak)
            || value == static_cast<int32_t>(PlayerAction::AbortBreak)
            || value == static_cast<int32_t>(PlayerAction::StartBreak)
            || value == static_cast<int32_t>(PlayerAction::CrackBreak)
            || value == static_cast<int32_t>(PlayerAction::PredictDestroyBlock)
            || value == static_cast<int32_t>(PlayerAction::ContinueDestroyBlock);
    }

    static bool isValidActionType(PlayerAction value) noexcept {
        return isValidActionType(static_cast<int32_t>(value));
    }

    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, PlayerAction value) {
        return PlayerBlockAction{
            value,
            stream.readSignedBlockPos().value_or(glm::ivec3(0)),
            stream.readVarInt().value_or(0)
        };
    }

    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, int32_t value) {
        return decodeWithBlockInfo(stream, static_cast<PlayerAction>(value));
    }
};

} // namespace bedrock::protocol
