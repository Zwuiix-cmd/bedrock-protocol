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
    int32_t recipeNetId;

    ShapedRecipe() = default;
    virtual ~ShapedRecipe() = default;

    static std::shared_ptr<ShapedRecipe> decode(const int32_t& recipeType, ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<ShapedRecipe>();
        result->recipeId = stream.readString().value_or("");
        result->priority = stream.readVarInt().value_or(0);
        result->symmetric = stream.readBool().value_or(false);
        result->blockName = stream.readString().value_or("");
        result->input = stream.readList([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
        result->output = stream.readList([&]() {
            NetworkItemDescriptor value;
            value.read(stream);
            return value;
        });
        result->uuid = stream.readUUID().value_or("");
        result->unlockingRequirement = std::make_shared<RecipeUnlockingRequirement>();
        result->unlockingRequirement->read(stream);
        result->recipeNetId = stream.readInt().value_or(0);
        return result;
    }

    void read(ReadOnlyBinaryStream& stream) override {
        recipeId = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        symmetric = stream.readBool().value_or(false);
        blockName = stream.readString().value_or("");
        input = stream.readList([&]() {
            return stream.readObjectList<RecipeIngredient>();
        });
        output = stream.readList([&]() {
            NetworkItemDescriptor value;
            value.read(stream);
            return value;
        });
        uuid = stream.readUUID().value_or("");
        unlockingRequirement = std::make_shared<RecipeUnlockingRequirement>();
        unlockingRequirement->read(stream);
        recipeNetId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
        stream.writeString(recipeId);
        stream.writeVarInt(priority);
        stream.writeBool(symmetric);
        stream.writeString(blockName);
        stream.writeList(input, [&](const auto& item) {
            stream.writeList(item, [&](const auto& item) {
                item->write(stream);
            });
        });
        stream.writeList(output, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeUUID(uuid);
        unlockingRequirement->write(stream);
        stream.writeInt(recipeNetId);
    }
};
