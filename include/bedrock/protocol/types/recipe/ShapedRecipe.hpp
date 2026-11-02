#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/recipe/RecipeUnlockingRequirement.hpp>
#include <bedrock/protocol/types/recipe/RecipeIngredient.hpp>
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>

class ShapedRecipe : public RecipeWithTypeId {
public:
    std::string blockName;
    std::string recipeId;
    std::vector<std::vector<std::shared_ptr<RecipeIngredient>>> input;
    std::vector<NetworkItemDescriptor> output;
    std::string uuid;
    int32_t priority;
    bool symmetric;
    std::shared_ptr<RecipeUnlockingRequirement> unlockingRequirement;
    uint32_t recipeNetId;

    ShapedRecipe() = default;
    virtual ~ShapedRecipe() = default;

    static std::shared_ptr<ShapedRecipe> create(const int32_t& recipeType, ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<ShapedRecipe>();
        result->blockName = stream.readString().value_or("");
        result->recipeId = stream.readString().value_or("");
        result->input = stream.readList([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
        result->output = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        result->uuid = stream.readUUID().value_or("");
        result->priority = stream.readVarInt().value_or(0);
        result->symmetric = stream.readBool().value_or(false);
        result->unlockingRequirement = std::make_shared<RecipeUnlockingRequirement>();
        result->unlockingRequirement->read(stream);
        result->recipeNetId = stream.readUnsignedVarInt().value_or(0);
        return result;
    }

    void read(ReadOnlyBinaryStream& stream) override {
        blockName = stream.readString().value_or("");
        recipeId = stream.readString().value_or("");
        input = stream.readList([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
        output = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        uuid = stream.readUUID().value_or("");
        priority = stream.readVarInt().value_or(0);
        symmetric = stream.readBool().value_or(false);
        unlockingRequirement = std::make_shared<RecipeUnlockingRequirement>();
        unlockingRequirement->read(stream);
        recipeNetId = stream.readUnsignedVarInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeString(blockName);
        stream.writeString(recipeId);
        stream.writeList(input, [&](const auto& item) {
            stream.writeList(item, [&](const auto& item) {
                item->write(stream);
            });
        });
        stream.writeList(output, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeUUID(uuid);
        stream.writeVarInt(priority);
        stream.writeBool(symmetric);
        unlockingRequirement->write(stream);
        stream.writeUnsignedVarInt(recipeNetId);
    }
};
