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
#include <bedrock/protocol/types/recipe/RecipeUnlockingContext.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingContext.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingContext.hpp>

class RecipeUnlockingRequirement {
public:
    RecipeUnlockingContext unlockingContext;
    std::optional<std::vector<std::shared_ptr<RecipeIngredient>>> unlockingIngredients;

    RecipeUnlockingRequirement() = default;
    explicit RecipeUnlockingRequirement(
        const RecipeUnlockingContext& unlockingContextValue,
        const std::optional<std::vector<std::shared_ptr<RecipeIngredient>>>& unlockingIngredientsValue
    ) :
        unlockingContext(unlockingContextValue),
        unlockingIngredients(unlockingIngredientsValue) {
		if (((unlockingContextValue != static_cast<std::underlying_type_t<RecipeUnlockingContext>>(RecipeUnlockingContext::None)) && (unlockingIngredientsValue != nullptr))) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        unlockingContext = static_cast<RecipeUnlockingContext>(stream.readVarInt().value_or(0));
        unlockingIngredients = stream.readOptional([&]() {
			return stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<RecipeIngredient>();
			value->read(stream);
			return value;
			}());
			});
		});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(static_cast<int32_t>(unlockingContext));
        stream.writeOptional(unlockingIngredients, [&](const auto& optionalValue) {
			stream.writeList(optionalValue, [&](const auto& item) {
			if (item) item->write(stream);
			});
		});
    }
};
