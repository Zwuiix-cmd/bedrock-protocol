#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>
namespace bedrock::protocol {


class CraftRecipeAutoStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 11;

    uint32_t recipeId;
    uint8_t repetitions;
    std::vector<std::shared_ptr<RecipeIngredient>> ingredients;

    CraftRecipeAutoStackRequestAction() = default;
    virtual ~CraftRecipeAutoStackRequestAction() = default;
    explicit CraftRecipeAutoStackRequestAction(
        const uint32_t& recipeIdValue,
        const uint8_t& repetitionsValue,
        const std::vector<std::shared_ptr<RecipeIngredient>>& ingredientsValue
    ) :
        recipeId(recipeIdValue),
        repetitions(repetitionsValue),
        ingredients(ingredientsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readUnsignedVarInt().value_or(0);
        repetitions = stream.readUnsignedByte().value_or(0);
        ingredients = stream.readList([&]() {
            auto ingredient = std::make_shared<RecipeIngredient>();
            ingredient->readStackRequest(stream);
            return ingredient;
        });
    }

    void write(BinaryStream& stream) const override {
        stream.writeUnsignedVarInt(recipeId);
        stream.writeUnsignedByte(repetitions);
        stream.writeList(ingredients, [&](const auto& item) {
            item->writeStackRequest(stream);
        });
    }
};

} // namespace bedrock::protocol
