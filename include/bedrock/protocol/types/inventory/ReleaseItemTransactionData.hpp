#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
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

    ReleaseItemTransactionData() = default;
    virtual ~ReleaseItemTransactionData() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        actionType = magic_enum::enum_cast<Action>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Action::Release);
        hotbarSlot = stream.readVarInt().value_or(0);
        headPosition = stream.readVec3().value_or(glm::vec3(0.0f));
        itemInHand = stream.readValue<NetworkItemStack>();
    }

    void write(BinaryStream& stream) const override {
        stream.writeInt(magic_enum::enum_integer(actionType));
        stream.writeVarInt(hotbarSlot);
        stream.writeVec3(headPosition);
        itemInHand.write(stream);
    }
};
