#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <optional>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
namespace bedrock::protocol {


class RecipeUnlockingRequirement {
public:
    std::optional<std::vector<std::shared_ptr<RecipeIngredient>>> unlockingIngredients;

    RecipeUnlockingRequirement() = default;
    explicit RecipeUnlockingRequirement(const std::optional<std::vector<std::shared_ptr<RecipeIngredient>>>& unlockingIngredientsValue) : unlockingIngredients(unlockingIngredientsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        unlockingIngredients = stream.readOptionalWhenFalse([&]() {
            return stream.readObjectList<RecipeIngredient>();
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

} // namespace bedrock::protocol
