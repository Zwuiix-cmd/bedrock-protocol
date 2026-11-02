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
#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>

class ShapedRecipe {
public:
    std::string blockName;
    std::string recipeId;
    std::vector<std::vector<std::shared_ptr<RecipeIngredient>>> input;
    std::vector<NetworkItemDescriptor> output;
    std::string uuid;
    int32_t priority;
    bool symmetric;
    std::optional<std::shared_ptr<RecipeUnlockingRequirement>> unlockingRequirement;
    int32_t recipeNetId;

    ShapedRecipe() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        recipeId = stream.readString().value_or("");
        uuid = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        symmetric = stream.readBool().value_or(false);
        blockName = stream.readString().value_or("");
        input = stream.readList([&]() {
			return stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<RecipeIngredient>();
			value->read(stream);
			return value;
			}());
			});
		});
        output = stream.readList([&]() {
			return ([&]() { NetworkItemDescriptor value; value.read(stream); return value; }());
		});
        unlockingRequirement = stream.readOptional([&]() {
			return ([&]() {
			auto value = std::make_shared<RecipeUnlockingRequirement>();
			value->read(stream);
			return value;
			}());
		});
        recipeNetId = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(recipeId);
        stream.writeString(uuid);
        stream.writeVarInt(priority);
        stream.writeBool(symmetric);
        stream.writeString(blockName);
        stream.writeList(input, [&](const auto& item) {
			stream.writeList(item, [&](const auto& item) {
			if (item) item->write(stream);
			});
		});
        stream.writeList(output, [&](const auto& item) {
			item.write(stream);
		});
        stream.writeOptional(unlockingRequirement, [&](const auto& optionalValue) {
			if (optionalValue) optionalValue->write(stream);
		});
        stream.writeInt(recipeNetId);
    }
};
