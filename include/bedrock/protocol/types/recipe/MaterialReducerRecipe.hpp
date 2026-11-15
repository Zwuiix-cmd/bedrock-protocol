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

    static std::shared_ptr<MaterialReducerRecipe> create(ReadOnlyBinaryStream& stream) {
		auto inputIdAndData = stream.readVarInt().value_or(0);
		auto inputId = (inputIdAndData >> 16);
		auto inputMeta = (inputIdAndData & 32767);
		auto outputs = stream.readList([&]() { return stream.readObject<MaterialReducerRecipeOutput>(); });
		return std::make_shared<MaterialReducerRecipe>(inputId, inputMeta, outputs);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(((inputItemId << 16) | inputItemMeta));
		stream.writeList(outputs, [&](const auto& output) {
			return output->write(stream);
		});
    }
};

} // namespace bedrock::protocol
