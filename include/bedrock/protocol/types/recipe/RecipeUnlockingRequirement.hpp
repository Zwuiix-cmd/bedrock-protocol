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
		if (((unlockingContextValue != magic_enum::enum_integer(RecipeUnlockingContext::None)) && (unlockingIngredientsValue != nullptr))) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        unlockingContext = magic_enum::enum_cast<RecipeUnlockingContext>(
            stream.readVarInt().value_or(0)
        ).value_or(RecipeUnlockingContext::None);
        unlockingIngredients = stream.readOptional([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(magic_enum::enum_integer(unlockingContext));
        stream.writeOptional(unlockingIngredients, [&](const auto& optionalValue) {
            stream.writeList(optionalValue, [&](const auto& item) {
                item->write(stream);
            });
        });
    }
};
