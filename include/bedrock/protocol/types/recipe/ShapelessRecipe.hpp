#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
namespace bedrock::protocol {


class ShapelessRecipe {
public:
    std::string recipeId;
    std::vector<std::shared_ptr<RecipeIngredient>> inputs;
    std::vector<NetworkItemDescriptor> outputs;
    std::string uuid;
    std::string blockName;
    int32_t priority;
    std::optional<std::shared_ptr<RecipeUnlockingRequirement>> unlockingRequirement;
    uint32_t recipeNetId;

    ShapelessRecipe() = default;
    explicit ShapelessRecipe(
        const std::string& recipeIdValue,
        const std::vector<std::shared_ptr<RecipeIngredient>>& inputsValue,
        const std::vector<NetworkItemDescriptor>& outputsValue,
        const std::string& uuidValue,
        const std::string& blockNameValue,
        const int32_t& priorityValue,
        const std::optional<std::shared_ptr<RecipeUnlockingRequirement>>& unlockingRequirementValue,
        const uint32_t& recipeNetIdValue
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
        inputs = stream.readObjectList<RecipeIngredient>();
        outputs = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        uuid = stream.readUUID().value_or("");
        blockName = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        unlockingRequirement = stream.readOptional([&]() {
            return stream.readObject<RecipeUnlockingRequirement>();
        });
        recipeNetId = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(recipeId);
        stream.writeList(inputs, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(outputs, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeUUID(uuid);
        stream.writeString(blockName);
        stream.writeVarInt(priority);
        stream.writeOptional(unlockingRequirement, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeUnsignedVarInt(recipeNetId);
    }
};

} // namespace bedrock::protocol
