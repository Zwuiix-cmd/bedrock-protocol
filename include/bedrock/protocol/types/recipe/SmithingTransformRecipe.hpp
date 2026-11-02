#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class SmithingTransformRecipe : public RecipeWithTypeId {
public:
    std::string recipeId;
    std::shared_ptr<RecipeIngredient> template_;
    std::shared_ptr<RecipeIngredient> input;
    std::shared_ptr<RecipeIngredient> addition;
    NetworkItemDescriptor output;
    std::string blockName;
    uint32_t recipeNetId;

    SmithingTransformRecipe() = default;
    virtual ~SmithingTransformRecipe() = default;
    explicit SmithingTransformRecipe(
        const int32_t& typeIdValue,
        const std::string& recipeIdValue,
        const std::shared_ptr<RecipeIngredient>& template_Value,
        const std::shared_ptr<RecipeIngredient>& inputValue,
        const std::shared_ptr<RecipeIngredient>& additionValue,
        const NetworkItemDescriptor& outputValue,
        const std::string& blockNameValue,
        const uint32_t& recipeNetIdValue
    ) :
        RecipeWithTypeId(typeIdValue),
        recipeId(recipeIdValue),
        template_(template_Value),
        input(inputValue),
        addition(additionValue),
        output(outputValue),
        blockName(blockNameValue),
        recipeNetId(recipeNetIdValue) {}

    static std::shared_ptr<SmithingTransformRecipe> create(const int32_t& typeId, ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<SmithingTransformRecipe>();
        result->recipeId = stream.readString().value_or("");
        result->template_ = std::make_shared<RecipeIngredient>();
        result->template_->read(stream);
        result->input = std::make_shared<RecipeIngredient>();
        result->input->read(stream);
        result->addition = std::make_shared<RecipeIngredient>();
        result->addition->read(stream);
        result->output = stream.readValue<NetworkItemDescriptor>();
        result->blockName = stream.readString().value_or("");
        result->recipeNetId = stream.readUnsignedVarInt().value_or(0);
        return result;
    }

    void read(ReadOnlyBinaryStream& stream) override {
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

    void write(BinaryStream& stream) const override {
        stream.writeString(recipeId);
        template_->write(stream);
        input->write(stream);
        addition->write(stream);
        output.write(stream);
        stream.writeString(blockName);
        stream.writeUnsignedVarInt(recipeNetId);
    }
};
