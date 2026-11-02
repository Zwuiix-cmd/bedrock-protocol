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
#include <bedrock/protocol/types/PlayerBlockAction.hpp>

class PlayerBlockActionWithBlockInfo {
public:
    int32_t actionType;
    glm::ivec3 blockPosition;
    int32_t face;

    PlayerBlockActionWithBlockInfo() = default;
    explicit PlayerBlockActionWithBlockInfo(const int32_t& actionTypeValue, const glm::ivec3& blockPositionValue, const int32_t& faceValue) : actionType(actionTypeValue), blockPosition(blockPositionValue), face(faceValue) {
		if (!PlayerBlockAction::isValidActionType(actionTypeValue)) {
			throw std::exception{};
		}
    }

    static std::shared_ptr<PlayerBlockActionWithBlockInfo> read(ReadOnlyBinaryStream& stream, const int32_t& actionType) {
		auto blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
		auto face = stream.readVarInt().value_or(0);
		return std::make_shared<PlayerBlockActionWithBlockInfo>(actionType, blockPosition, face);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readVarInt().value_or(0);
        actionType = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeSignedBlockPos(blockPosition);
		stream.writeVarInt(face);
    }
};
