#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/recipe/MaterialReducerRecipeOutput.hpp>
namespace bedrock::protocol {


class MaterialReducerRecipe {
public:
    int32_t inputItemId;
    int32_t inputItemMeta;
    std::vector<std::shared_ptr<MaterialReducerRecipeOutput>> outputs;

    MaterialReducerRecipe() = default;
    explicit MaterialReducerRecipe(
        const int32_t& inputItemIdValue,
        const int32_t& inputItemMetaValue,
        const std::vector<std::shared_ptr<MaterialReducerRecipeOutput>>& outputsValue
    ) :
        inputItemId(inputItemIdValue),
        inputItemMeta(inputItemMetaValue),
        outputs(outputsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        inputItemId = stream.readVarInt().value_or(0);
        inputItemMeta = stream.readVarInt().value_or(0);
        outputs = stream.readObjectList<MaterialReducerRecipeOutput>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(inputItemId);
        stream.writeVarInt(inputItemMeta);
        stream.writeList(outputs, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
