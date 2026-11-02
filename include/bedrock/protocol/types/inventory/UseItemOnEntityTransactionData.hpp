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
#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>

class UseItemOnEntityTransactionData : public TransactionData {
public:
    static constexpr int64_t ID = 3;
    static constexpr int64_t ACTION_INTERACT = 0;
    static constexpr int64_t ACTION_ATTACK = 1;
    static constexpr int64_t ACTION_ITEM_INTERACT = 2;

    uint64_t actorRuntimeId;
    int32_t actionType;
    int32_t hotbarSlot;
    HandSlot hand;
    NetworkItemStack itemInHand;
    glm::vec3 playerPosition;
    glm::vec3 clickPosition;

    UseItemOnEntityTransactionData() = default;
    virtual ~UseItemOnEntityTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actorRuntimeId = stream.readActorRuntimeID().value_or(0);
        actionType = stream.readVarInt().value_or(0);
        hotbarSlot = stream.readVarInt().value_or(0);
        playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        clickPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        hand = static_cast<HandSlot>(stream.readVarInt().value_or(0));
        itemInHand = ([&]() { NetworkItemStack value; value.read(stream); return value; }());
    }

    void write(BinaryStream& stream) const override {
        stream.writeActorRuntimeID(actorRuntimeId);
        stream.writeVarInt(actionType);
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(playerPosition);
        stream.writeVec3(clickPosition);
        stream.writeVarInt(static_cast<int32_t>(hand));
        itemInHand.write(stream);
    }
};
