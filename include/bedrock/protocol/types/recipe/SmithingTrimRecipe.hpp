#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class SmithingTrimRecipe : public RecipeWithTypeId {
public:
    std::string recipeId;
    std::shared_ptr<RecipeIngredient> template_;
    std::shared_ptr<RecipeIngredient> input;
    std::shared_ptr<RecipeIngredient> addition;
    std::string blockName;
    int32_t recipeNetId;

    SmithingTrimRecipe() = default;
    virtual ~SmithingTrimRecipe() = default;
    explicit SmithingTrimRecipe(
        const int32_t& typeIdValue,
        const std::string& recipeIdValue,
        const std::shared_ptr<RecipeIngredient>& template_Value,
        const std::shared_ptr<RecipeIngredient>& inputValue,
        const std::shared_ptr<RecipeIngredient>& additionValue,
        const std::string& blockNameValue,
        const int32_t& recipeNetIdValue
    ) :
        RecipeWithTypeId(typeIdValue),
        recipeId(recipeIdValue),
        template_(template_Value),
        input(inputValue),
        addition(additionValue),
        blockName(blockNameValue),
        recipeNetId(recipeNetIdValue) {
		(void)0;
    }

    static std::shared_ptr<SmithingTrimRecipe> create(const int32_t& typeId, ReadOnlyBinaryStream& stream) {
		auto recipeId = stream.readString().value_or("");
		auto template_ = stream.readObject<RecipeIngredient>();
		auto input = stream.readObject<RecipeIngredient>();
		auto addition = stream.readObject<RecipeIngredient>();
		auto blockName = stream.readString().value_or("");
		auto recipeNetId = stream.readUnsignedVarInt().value_or(0);
		return std::make_shared<SmithingTrimRecipe>(typeId, recipeId, template_, input, addition, blockName, recipeNetId);
    }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readString().value_or("");
        blockName = stream.readString().value_or("");
        template_ = std::make_shared<RecipeIngredient>();
        template_->read(stream);
        input = std::make_shared<RecipeIngredient>();
        input->read(stream);
        addition = std::make_shared<RecipeIngredient>();
        addition->read(stream);
        recipeNetId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeString(recipeId);
		if (template_) template_->write(stream);
		if (input) input->write(stream);
		if (addition) addition->write(stream);
		stream.writeString(blockName);
		stream.writeUnsignedVarInt(recipeNetId);
    }
};
