#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class GrindstoneStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 14;

    uint32_t recipeId;
    int32_t repairCost;
    uint8_t repetitions;

    GrindstoneStackRequestAction() = default;
    virtual ~GrindstoneStackRequestAction() = default;
    explicit GrindstoneStackRequestAction(const uint32_t& recipeIdValue, const int32_t& repairCostValue, const uint8_t& repetitionsValue) : recipeId(recipeIdValue), repairCost(repairCostValue), repetitions(repetitionsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readUnsignedInt().value_or(0);
        repairCost = stream.readVarInt().value_or(0);
        repetitions = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedInt(recipeId);
		stream.writeUnsignedByte(repetitions);
		stream.writeVarInt(repairCost);
    }
};

} // namespace bedrock::protocol
