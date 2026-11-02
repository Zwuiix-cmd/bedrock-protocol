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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

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
        stackId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVarInt(hotbarSlot);
		stream.writeVarInt(predictedDurability);
		stream.writeInt(stackId);
    }
};
