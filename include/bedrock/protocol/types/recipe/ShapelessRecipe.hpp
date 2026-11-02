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
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>

class ShapelessRecipe {
public:
    std::string recipeId;
    std::vector<std::shared_ptr<RecipeIngredient>> inputs;
    std::vector<NetworkItemDescriptor> outputs;
    std::string uuid;
    std::string blockName;
    int32_t priority;
    std::optional<std::shared_ptr<RecipeUnlockingRequirement>> unlockingRequirement;
    int32_t recipeNetId;

    ShapelessRecipe() = default;
    explicit ShapelessRecipe(
        const std::string& recipeIdValue,
        const std::vector<std::shared_ptr<RecipeIngredient>>& inputsValue,
        const std::vector<NetworkItemDescriptor>& outputsValue,
        const std::string& uuidValue,
        const std::string& blockNameValue,
        const int32_t& priorityValue,
        const std::optional<std::shared_ptr<RecipeUnlockingRequirement>>& unlockingRequirementValue,
        const int32_t& recipeNetIdValue
    ) :
        recipeId(recipeIdValue),
        inputs(inputsValue),
        outputs(outputsValue),
        uuid(uuidValue),
        blockName(blockNameValue),
        priority(priorityValue),
        unlockingRequirement(unlockingRequirementValue),
        recipeNetId(recipeNetIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        recipeId = stream.readString().value_or("");
        blockName = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        inputs = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<RecipeIngredient>();
			value->read(stream);
			return value;
			}());
		});
        outputs = stream.readList([&]() {
			return ([&]() { NetworkItemDescriptor value; value.read(stream); return value; }());
		});
        uuid = stream.readUUID().value_or("");
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
        stream.writeString(blockName);
        stream.writeVarInt(priority);
        stream.writeList(inputs, [&](const auto& item) {
			if (item) item->write(stream);
		});
        stream.writeList(outputs, [&](const auto& item) {
			item.write(stream);
		});
        stream.writeUUID(uuid);
        stream.writeOptional(unlockingRequirement, [&](const auto& optionalValue) {
			if (optionalValue) optionalValue->write(stream);
		});
        stream.writeInt(recipeNetId);
    }
};
