#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>
namespace bedrock::protocol {


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
        actions = stream.readObjectList<NetworkInventoryAction>();
        actionType = magic_enum::enum_cast<Action>(
            stream.readVarInt().value_or(0)
        ).value_or(Action::Release);
        hotbarSlot = stream.readVarInt().value_or(0);
        itemInHand = stream.readValue<NetworkItemStack>();
        headPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        hand = magic_enum::enum_cast<HandSlot>(stream.readUnsignedByte().value_or(0)).value_or(HandSlot::Mainhand);
    }

    void write(BinaryStream& stream) const override {
        stream.writeList(actions, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeVarInt(magic_enum::enum_integer(actionType));
        stream.writeVarInt(hotbarSlot);
        itemInHand.write(stream);
        stream.writeVec3(headPosition);
        stream.writeUnsignedByte(static_cast<uint8_t>(hand));
    }
};

} // namespace bedrock::protocol
