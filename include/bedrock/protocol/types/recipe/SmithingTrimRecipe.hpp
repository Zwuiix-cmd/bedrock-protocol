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
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>

class SmithingTrimRecipe {
public:
    std::string recipeId;
    std::shared_ptr<RecipeIngredient> template_;
    std::shared_ptr<RecipeIngredient> input;
    std::shared_ptr<RecipeIngredient> addition;
    std::string blockName;
    int32_t recipeNetId;

    SmithingTrimRecipe() = default;
    explicit SmithingTrimRecipe(
        const std::string& recipeIdValue,
        const std::shared_ptr<RecipeIngredient>& template_Value,
        const std::shared_ptr<RecipeIngredient>& inputValue,
        const std::shared_ptr<RecipeIngredient>& additionValue,
        const std::string& blockNameValue,
        const int32_t& recipeNetIdValue
    ) :
        recipeId(recipeIdValue),
        template_(template_Value),
        input(inputValue),
        addition(additionValue),
        blockName(blockNameValue),
        recipeNetId(recipeNetIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        recipeId = stream.readString().value_or("");
        blockName = stream.readString().value_or("");
        template_ = ([&]() {
    auto value = std::make_shared<RecipeIngredient>();
    value->read(stream);
    return value;
}());
        input = ([&]() {
    auto value = std::make_shared<RecipeIngredient>();
    value->read(stream);
    return value;
}());
        addition = ([&]() {
    auto value = std::make_shared<RecipeIngredient>();
    value->read(stream);
    return value;
}());
        recipeNetId = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(recipeId);
		if (template_) template_->write(stream);
		if (input) input->write(stream);
		if (addition) addition->write(stream);
		stream.writeString(blockName);
		stream.writeUnsignedVarInt(recipeNetId);
    }
};
