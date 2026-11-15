#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
namespace bedrock::protocol {


class SmithingTransformRecipe {
public:
    std::string recipeId;
    std::shared_ptr<RecipeIngredient> template_;
    std::shared_ptr<RecipeIngredient> input;
    std::shared_ptr<RecipeIngredient> addition;
    NetworkItemDescriptor output;
    std::string blockName;
    uint32_t recipeNetId;

    SmithingTransformRecipe() = default;
    explicit SmithingTransformRecipe(
        const std::string& recipeIdValue,
        const std::shared_ptr<RecipeIngredient>& template_Value,
        const std::shared_ptr<RecipeIngredient>& inputValue,
        const std::shared_ptr<RecipeIngredient>& additionValue,
        const NetworkItemDescriptor& outputValue,
        const std::string& blockNameValue,
        const uint32_t& recipeNetIdValue
    ) :
        recipeId(recipeIdValue),
        template_(template_Value),
        input(inputValue),
        addition(additionValue),
        output(outputValue),
        blockName(blockNameValue),
        recipeNetId(recipeNetIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        recipeId = stream.readString().value_or("");
        template_ = std::make_shared<RecipeIngredient>();
        template_->read(stream);
        input = std::make_shared<RecipeIngredient>();
        input->read(stream);
        addition = std::make_shared<RecipeIngredient>();
        addition->read(stream);
        output = stream.readValue<NetworkItemDescriptor>();
        blockName = stream.readString().value_or("");
        recipeNetId = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(recipeId);
        template_->write(stream);
        input->write(stream);
        addition->write(stream);
        output.write(stream);
        stream.writeString(blockName);
        stream.writeUnsignedVarInt(recipeNetId);
    }
};

} // namespace bedrock::protocol
