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
#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>

class UseItemTransactionData : public TransactionData {
public:
    enum class Action : uint8_t {
        ClickBlock = 0,
        ClickAir = 1,
        BreakBlock = 2,
        UseAsAttack = 3,
    };

    static constexpr int64_t ID = 2;

    Action actionType;
    TriggerType triggerType;
    glm::ivec3 blockPosition;
    int32_t face;
    int32_t hotbarSlot;
    NetworkItemStack itemInHand;
    glm::vec3 playerPosition;
    glm::vec3 clickPosition;
    uint32_t blockRuntimeId;
    PredictedResult clientInteractPrediction;
    uint8_t clientCooldownState;

    UseItemTransactionData() = default;
    virtual ~UseItemTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actionType = magic_enum::enum_cast<Action>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Action::ClickBlock);
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readVarInt().value_or(0);
        hotbarSlot = stream.readVarInt().value_or(0);
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        clientCooldownState = stream.readUnsignedByte().value_or(0);
        triggerType = magic_enum::enum_cast<TriggerType>(
            stream.readVarInt().value_or(0)
        ).value_or(TriggerType::Unknown);
        itemInHand.read(stream);
        clientInteractPrediction = magic_enum::enum_cast<PredictedResult>(
            stream.readVarInt().value_or(0)
        ).value_or(PredictedResult::Failure);
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedVarInt(actionType);
        stream.writeSignedBlockPos(blockPosition);
        stream.writeVarInt(face);
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
        stream.writeUnsignedVarInt(blockRuntimeId);
        stream.writeUnsignedByte(clientCooldownState);
        stream.writeVarInt(magic_enum::enum_integer(triggerType));
        itemInHand.write(stream);
        stream.writeVarInt(magic_enum::enum_integer(clientInteractPrediction));
    }
};
