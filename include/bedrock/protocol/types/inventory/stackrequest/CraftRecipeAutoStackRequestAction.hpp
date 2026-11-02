#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeAutoStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 13;

    int32_t recipeId;
    uint8_t repetitions;
    uint8_t repetitions2;
    std::vector<std::shared_ptr<RecipeIngredient>> ingredients;

    CraftRecipeAutoStackRequestAction() = default;
    virtual ~CraftRecipeAutoStackRequestAction() = default;
    explicit CraftRecipeAutoStackRequestAction(
        const int32_t& recipeIdValue,
        const uint8_t& repetitionsValue,
        const uint8_t& repetitions2Value,
        const std::vector<std::shared_ptr<RecipeIngredient>>& ingredientsValue
    ) :
        recipeId(recipeIdValue),
        repetitions(repetitionsValue),
        repetitions2(repetitions2Value),
        ingredients(ingredientsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readInt().value_or(0);
        repetitions = stream.readUnsignedByte().value_or(0);
        repetitions2 = stream.readUnsignedByte().value_or(0);
        ingredients = stream.readObjectList<RecipeIngredient>();
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(recipeId);
		stream.writeUnsignedByte(repetitions);
		stream.writeUnsignedByte(repetitions2);
		stream.writeUnsignedByte(ingredients.size());
		for (const auto& ingredient : ingredients) {
			if (ingredient) ingredient->write(stream);
		}
    }
};
