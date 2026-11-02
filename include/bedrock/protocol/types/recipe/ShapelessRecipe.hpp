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
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class ShapelessRecipe : public RecipeWithTypeId {
public:
    std::string recipeId;
    std::vector<std::shared_ptr<RecipeIngredient>> inputs;
    std::vector<NetworkItemDescriptor> outputs;
    std::string uuid;
    std::string blockName;
    int32_t priority;
    std::shared_ptr<RecipeUnlockingRequirement> unlockingRequirement;
    int32_t recipeNetId;

    ShapelessRecipe() = default;
    virtual ~ShapelessRecipe() = default;
    explicit ShapelessRecipe(
        const int32_t& typeIdValue,
        const std::string& recipeIdValue,
        const std::vector<std::shared_ptr<RecipeIngredient>>& inputsValue,
        const std::vector<NetworkItemDescriptor>& outputsValue,
        const std::string& uuidValue,
        const std::string& blockNameValue,
        const int32_t& priorityValue,
        const std::shared_ptr<RecipeUnlockingRequirement>& unlockingRequirementValue,
        const int32_t& recipeNetIdValue
    ) :
        RecipeWithTypeId(typeIdValue),
        recipeId(recipeIdValue),
        inputs(inputsValue),
        outputs(outputsValue),
        uuid(uuidValue),
        blockName(blockNameValue),
        priority(priorityValue),
        unlockingRequirement(unlockingRequirementValue),
        recipeNetId(recipeNetIdValue) {
		(void)0;
    }

    static std::shared_ptr<ShapelessRecipe> decode(const int32_t& recipeType, ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<ShapelessRecipe>();
        result->recipeId = stream.readString().value_or("");
        result->blockName = stream.readString().value_or("");
        result->priority = stream.readVarInt().value_or(0);
        result->inputs = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<RecipeIngredient>();
            value->read(stream);
            return value;
            }());
        });
        result->outputs = stream.readList([&]() {
            return ([&]() { NetworkItemDescriptor value; value.read(stream); return value; }());
        });
        result->uuid = stream.readUUID().value_or("");
        result->unlockingRequirement = ([&]() {
    auto value = std::make_shared<RecipeUnlockingRequirement>();
    value->read(stream);
    return value;
}());
        result->recipeNetId = stream.readInt().value_or(0);
        return result;
    }

    void read(ReadOnlyBinaryStream& stream) override {
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
        unlockingRequirement = ([&]() {
    auto value = std::make_shared<RecipeUnlockingRequirement>();
    value->read(stream);
    return value;
}());
        recipeNetId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
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
        if (unlockingRequirement) unlockingRequirement->write(stream);
        stream.writeInt(recipeNetId);
    }
};
