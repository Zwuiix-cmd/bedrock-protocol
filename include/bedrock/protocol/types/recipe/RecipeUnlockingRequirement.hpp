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

class RecipeUnlockingRequirement {
public:
    std::optional<std::vector<std::shared_ptr<RecipeIngredient>>> unlockingIngredients;

    RecipeUnlockingRequirement() = default;
    explicit RecipeUnlockingRequirement(const std::optional<std::vector<std::shared_ptr<RecipeIngredient>>>& unlockingIngredientsValue) : unlockingIngredients(unlockingIngredientsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
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
		stream.writeBool((!unlockingIngredients));
		if (unlockingIngredients.has_value()) {
			stream.writeUnsignedVarInt(unlockingIngredients.value().size());
			for (const auto& ingredient : unlockingIngredients.value()) {
				if (ingredient) ingredient->write(stream);
			}
		}
    }
};
