#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeOptionalStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 15;

    int32_t recipeId;
    int32_t filterStringIndex;

    CraftRecipeOptionalStackRequestAction() = default;
    virtual ~CraftRecipeOptionalStackRequestAction() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readInt().value_or(0);
        filterStringIndex = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(recipeId);
		stream.writeInt(filterStringIndex);
    }
};
