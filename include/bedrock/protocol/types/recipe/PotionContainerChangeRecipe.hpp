#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class PotionContainerChangeRecipe {
public:
    int32_t inputItemId;
    int32_t ingredientItemId;
    int32_t outputItemId;

    PotionContainerChangeRecipe() = default;
    explicit PotionContainerChangeRecipe(
        const int32_t& inputItemIdValue,
        const int32_t& ingredientItemIdValue,
        const int32_t& outputItemIdValue
    ) :
        inputItemId(inputItemIdValue),
        ingredientItemId(ingredientItemIdValue),
        outputItemId(outputItemIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        inputItemId = stream.readVarInt().value_or(0);
        ingredientItemId = stream.readVarInt().value_or(0);
        outputItemId = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(inputItemId);
        stream.writeVarInt(ingredientItemId);
        stream.writeVarInt(outputItemId);
    }
};
