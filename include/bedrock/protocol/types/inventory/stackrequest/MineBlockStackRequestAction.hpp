#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class MineBlockStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 9;

    int32_t hotbarSlot;
    int32_t predictedDurability;
    int32_t stackId;

    MineBlockStackRequestAction() = default;
    virtual ~MineBlockStackRequestAction() = default;
    explicit MineBlockStackRequestAction(const int32_t& hotbarSlotValue, const int32_t& predictedDurabilityValue, const int32_t& stackIdValue) : hotbarSlot(hotbarSlotValue), predictedDurability(predictedDurabilityValue), stackId(stackIdValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        hotbarSlot = stream.readVarInt().value_or(0);
        predictedDurability = stream.readVarInt().value_or(0);
        stackId = stream.readVarInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVarInt(hotbarSlot);
		stream.writeVarInt(predictedDurability);
		stream.writeVarInt(stackId);
    }
};

} // namespace bedrock::protocol
