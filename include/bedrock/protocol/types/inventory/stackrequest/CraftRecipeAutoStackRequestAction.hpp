#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeAutoStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 13;

    uint32_t recipeId;
    uint8_t repetitions;
    uint8_t repetitions2;
    std::vector<std::shared_ptr<RecipeIngredient>> ingredients;

    CraftRecipeAutoStackRequestAction() = default;
    virtual ~CraftRecipeAutoStackRequestAction() = default;
    explicit CraftRecipeAutoStackRequestAction(
        const uint32_t& recipeIdValue,
        const uint8_t& repetitionsValue,
        const uint8_t& repetitions2Value,
        const std::vector<std::shared_ptr<RecipeIngredient>>& ingredientsValue
    ) :
        recipeId(recipeIdValue),
        repetitions(repetitionsValue),
        repetitions2(repetitions2Value),
        ingredients(ingredientsValue) {}

    static std::shared_ptr<CraftRecipeAutoStackRequestAction> create(ReadOnlyBinaryStream& stream) {
		auto recipeId = stream.readUnsignedVarInt().value_or(0);
		auto repetitions = stream.readUnsignedByte().value_or(0);
		auto repetitions2 = stream.readUnsignedByte().value_or(0);
		auto ingredients = std::vector<std::shared_ptr<RecipeIngredient>>();
		auto ingredientsCount = stream.readUnsignedByte().value_or(0);
		for (std::size_t i = 0; i < ingredientsCount; ++i) {
		    ingredients.push_back(stream.readObject<RecipeIngredient>());
		}
		return std::make_shared<CraftRecipeAutoStackRequestAction>(static_cast<int32_t>(recipeId), static_cast<int32_t>(repetitions), static_cast<int32_t>(repetitions2), ingredients);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
