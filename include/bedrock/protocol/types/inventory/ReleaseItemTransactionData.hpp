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

class ReleaseItemTransactionData : public TransactionData {
public:
    enum class Action : uint8_t {
        Release = 0,
        Consume = 1,
    };

    static constexpr int64_t ID = 4;

    Action actionType;
    int32_t hotbarSlot;
    NetworkItemStack itemInHand;
    glm::vec3 headPosition;
    HandSlot hand;

    ReleaseItemTransactionData() = default;
    virtual ~ReleaseItemTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actionType = magic_enum::enum_cast<Action>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<Action>(0));
        hotbarSlot = stream.readVarInt().value_or(0);
        headPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        itemInHand.read(stream);
        hand = magic_enum::enum_cast<HandSlot>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<HandSlot>(0));
    }

    void write(BinaryStream& stream) const override {
        stream.writeVarInt(magic_enum::enum_integer(actionType));
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(headPosition);
        itemInHand.write(stream);
        stream.writeVarInt(magic_enum::enum_integer(hand));
    }
};
