#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 12;

    int32_t recipeId;
    uint8_t repetitions;

    CraftRecipeStackRequestAction() = default;
    virtual ~CraftRecipeStackRequestAction() = default;
    explicit CraftRecipeStackRequestAction(const int32_t& recipeIdValue, const uint8_t& repetitionsValue) : recipeId(recipeIdValue), repetitions(repetitionsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readInt().value_or(0);
        repetitions = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(recipeId);
		stream.writeUnsignedByte(repetitions);
    }
};
