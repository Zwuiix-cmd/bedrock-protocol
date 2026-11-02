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
#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
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
    uint8_t face;
    int32_t hotbarSlot;
    HandSlot hand;
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
        actionType = static_cast<Action>(stream.readVarInt().value_or(0));
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readUnsignedByte().value_or(0);
        hotbarSlot = stream.readVarInt().value_or(0);
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        clientCooldownState = stream.readUnsignedByte().value_or(0);
        triggerType = static_cast<TriggerType>(stream.readVarInt().value_or(0));
        hand = static_cast<HandSlot>(stream.readVarInt().value_or(0));
        itemInHand = ([&]() { NetworkItemStack value; value.read(stream); return value; }());
        clientInteractPrediction = static_cast<PredictedResult>(stream.readVarInt().value_or(0));
    }

    void write(BinaryStream& stream) const override {
        stream.writeVarInt(static_cast<std::underlying_type_t<Action>>(actionType));
        stream.writeSignedBlockPos(blockPosition);
        stream.writeUnsignedByte(face);
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
        stream.writeUnsignedVarInt(blockRuntimeId);
        stream.writeUnsignedByte(clientCooldownState);
        stream.writeVarInt(static_cast<int32_t>(triggerType));
        stream.writeVarInt(static_cast<int32_t>(hand));
        itemInHand.write(stream);
        stream.writeVarInt(static_cast<int32_t>(clientInteractPrediction));
    }
};
