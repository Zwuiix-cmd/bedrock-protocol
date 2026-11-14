#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>
namespace bedrock::protocol {


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
        actions = stream.readObjectList<NetworkInventoryAction>();
        actionType = magic_enum::enum_cast<Action>(
            stream.readVarInt().value_or(0)
        ).value_or(Action::ClickBlock);
        triggerType = magic_enum::enum_cast<TriggerType>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(TriggerType::Unknown);
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        face = stream.readUnsignedByte().value_or(0);
        hotbarSlot = stream.readVarInt().value_or(0);
        hand = magic_enum::enum_cast<HandSlot>(stream.readUnsignedByte().value_or(0)).value_or(HandSlot::Mainhand);
        itemInHand = stream.readValue<NetworkItemStack>();
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        clientInteractPrediction = magic_enum::enum_cast<PredictedResult>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(PredictedResult::Failure);
        clientCooldownState = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(actions, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeVarInt(magic_enum::enum_integer(actionType));
        stream.writeUnsignedByte(magic_enum::enum_integer(triggerType));
        stream.writeSignedBlockPos(blockPosition);
        stream.writeUnsignedByte(face);
        stream.writeVarInt(hotbarSlot);
        stream.writeUnsignedByte(static_cast<uint8_t>(hand));
        itemInHand.write(stream);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
        stream.writeUnsignedVarInt(blockRuntimeId);
        stream.writeUnsignedByte(magic_enum::enum_integer(clientInteractPrediction));
        stream.writeUnsignedByte(clientCooldownState);
    }
};

} // namespace bedrock::protocol
