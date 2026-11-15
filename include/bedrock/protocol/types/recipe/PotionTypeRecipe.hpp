#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class PotionTypeRecipe {
public:
    int32_t inputItemId;
    int32_t inputItemMeta;
    int32_t ingredientItemId;
    int32_t ingredientItemMeta;
    int32_t outputItemId;
    int32_t outputItemMeta;

    PotionTypeRecipe() = default;
    explicit PotionTypeRecipe(
        const int32_t& inputItemIdValue,
        const int32_t& inputItemMetaValue,
        const int32_t& ingredientItemIdValue,
        const int32_t& ingredientItemMetaValue,
        const int32_t& outputItemIdValue,
        const int32_t& outputItemMetaValue
    ) :
        inputItemId(inputItemIdValue),
        inputItemMeta(inputItemMetaValue),
        ingredientItemId(ingredientItemIdValue),
        ingredientItemMeta(ingredientItemMetaValue),
        outputItemId(outputItemIdValue),
        outputItemMeta(outputItemMetaValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        inputItemId = stream.readVarInt().value_or(0);
        inputItemMeta = stream.readVarInt().value_or(0);
        ingredientItemId = stream.readVarInt().value_or(0);
        ingredientItemMeta = stream.readVarInt().value_or(0);
        outputItemId = stream.readVarInt().value_or(0);
        outputItemMeta = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(inputItemId);
		stream.writeVarInt(inputItemMeta);
		stream.writeVarInt(ingredientItemId);
		stream.writeVarInt(ingredientItemMeta);
		stream.writeVarInt(outputItemId);
		stream.writeVarInt(outputItemMeta);
    }
};

} // namespace bedrock::protocol
