#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>

class UseItemOnEntityTransactionData : public TransactionData {
public:
    enum class Action : uint8_t {
        Interact = 0,
        Attack = 1,
        ItemInteract = 2,
    };

    static constexpr int64_t ID = 3;

    uint64_t actorRuntimeId;
    Action actionType;
    int32_t hotbarSlot;
    NetworkItemStack itemInHand;
    glm::vec3 playerPosition;
    glm::vec3 clickPosition;

    UseItemOnEntityTransactionData() = default;
    virtual ~UseItemOnEntityTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actions = stream.readObjectList<NetworkInventoryAction>();
        actorRuntimeId = stream.readActorRuntimeID().value_or(0);
        actionType = magic_enum::enum_cast<Action>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Action::Interact);
        hotbarSlot = stream.readVarInt().value_or(0);
        itemInHand = stream.readValue<NetworkItemStack>();
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(actions, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeActorRuntimeID(actorRuntimeId);
        stream.writeUnsignedVarInt(magic_enum::enum_integer(actionType));
        stream.writeVarInt(hotbarSlot);
        itemInHand.write(stream);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
    }
};
