#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>
#include <memory>

#include <bedrock/protocol/types/PlayerBlockAction.hpp>
namespace bedrock::protocol {


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

    static std::shared_ptr<PlayerBlockActionWithBlockInfo> create(ReadOnlyBinaryStream& stream, const int32_t& actionType) {
		auto blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
		auto face = stream.readVarInt().value_or(0);
		return std::make_shared<PlayerBlockActionWithBlockInfo>(actionType, blockPosition, face);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        actionType = stream.readInt().value_or(0);
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeSignedBlockPos(blockPosition);
		stream.writeVarInt(face);
    }
};

} // namespace bedrock::protocol
