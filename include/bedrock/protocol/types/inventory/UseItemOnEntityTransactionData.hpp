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
        actorRuntimeId = stream.readActorRuntimeID().value_or(0);
        actionType = magic_enum::enum_cast<Action>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Action::Interact);
        hotbarSlot = stream.readVarInt().value_or(0);
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        itemInHand.read(stream);
    }

    void write(BinaryStream& stream) const override {
        stream.writeActorRuntimeID(actorRuntimeId);
        stream.writeUnsignedVarInt(actionType);
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
        itemInHand.write(stream);
    }
};
