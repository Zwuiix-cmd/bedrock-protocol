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


    void write(BinaryStream& stream) const {
        if (actionType != PlayerAction::StopBreak && isValidActionType(actionType)) {
            stream.writeSignedBlockPos(blockPosition);
            stream.writeVarInt(face);
        }
    }

    void read(ReadOnlyBinaryStream& stream) {
        if (actionType != PlayerAction::StopBreak && isValidActionType(actionType)) {
            blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
            face = stream.readVarInt().value_or(0);
        }
    }

    static bool isValidActionType(PlayerAction value) noexcept {
        return value == PlayerAction::AbortBreak || value == PlayerAction::StartBreak || value == PlayerAction::CrackBreak || value == PlayerAction::PredictDestroyBlock || value == PlayerAction::ContinueDestroyBlock;
    }

    static bool isValidActionType(int32_t value) noexcept {
        return isValidActionType(static_cast<PlayerAction>(value));
    }

    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, PlayerAction value) {
        PlayerBlockAction result;
        result.actionType = value;
        result.read(stream);
        return result;
    }
    static PlayerBlockAction decodeWithBlockInfo(ReadOnlyBinaryStream& stream, int32_t value) {
        return decodeWithBlockInfo(stream, static_cast<PlayerAction>(value));
    }
};

} // namespace bedrock::protocol
