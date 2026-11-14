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
    uint32_t recipeNetId;

    ShapedRecipe() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        recipeId = stream.readString().value_or("");
        input = stream.readList([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
        output = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        uuid = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        symmetric = stream.readBool().value_or(false);
        unlockingRequirement = stream.readOptional([&]() {
            return stream.readObject<RecipeUnlockingRequirement>();
        });
        recipeNetId = stream.readUnsignedVarInt().value_or(0);
        blockName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(recipeId);
        stream.writeList(input, [&](const auto& item) {
            stream.writeList(item, [&](const auto& item) {
                item->write(stream);
            });
        });
        stream.writeList(output, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeString(uuid);
        stream.writeVarInt(priority);
        stream.writeBool(symmetric);
        stream.writeOptional(unlockingRequirement, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeUnsignedVarInt(recipeNetId);
        stream.writeString(blockName);
    }
};

} // namespace bedrock::protocol
