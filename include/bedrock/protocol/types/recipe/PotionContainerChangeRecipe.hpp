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
