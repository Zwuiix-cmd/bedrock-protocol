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
#include <bedrock/protocol/types/recipe/MaterialReducerRecipeOutput.hpp>

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
        outputs = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<MaterialReducerRecipeOutput>();
            value->read(stream);
            return value;
            }());
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(inputItemId);
        stream.writeVarInt(inputItemMeta);
        stream.writeList(outputs, [&](const auto& item) {
            if (item) item->write(stream);
        });
    }
};
