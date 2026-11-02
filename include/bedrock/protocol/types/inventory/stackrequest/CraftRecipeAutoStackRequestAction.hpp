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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeAutoStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 11;

    int32_t recipeId;
    uint8_t repetitions;
    std::vector<std::shared_ptr<RecipeIngredient>> ingredients;

    CraftRecipeAutoStackRequestAction() = default;
    virtual ~CraftRecipeAutoStackRequestAction() = default;
    explicit CraftRecipeAutoStackRequestAction(
        const int32_t& recipeIdValue,
        const uint8_t& repetitionsValue,
        const std::vector<std::shared_ptr<RecipeIngredient>>& ingredientsValue
    ) :
        recipeId(recipeIdValue),
        repetitions(repetitionsValue),
        ingredients(ingredientsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        repetitions = stream.readUnsignedByte().value_or(0);
        recipeId = stream.readInt().value_or(0);
        ingredients = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<RecipeIngredient>();
	value->read(stream);
	return value;
	}());
});
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedByte(repetitions);
        stream.writeInt(recipeId);
        stream.writeList(ingredients, [&](const auto& item) {
	if (item) item->write(stream);
});
    }
};
