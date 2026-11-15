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
        const auto width = stream.readVarInt().value_or(0);
        const auto height = stream.readVarInt().value_or(0);
        const auto count = stream.readUnsignedVarInt().value_or(0);
        if (width < 0 || height < 1 || height > 3
            || count != static_cast<uint64_t>(width) * height
            || !stream.canReadCollection(count)) {
            stream.markOverflowed();
            return;
        }
        input.assign(static_cast<std::size_t>(height), {});
        for (auto& row : input) {
            row.reserve(static_cast<std::size_t>(width));
            for (int32_t column = 0; column < width && !stream.hasOverflowed(); ++column) {
                row.push_back(stream.readObject<RecipeIngredient>());
            }
        }
        output = stream.readList([&]() {
            return stream.readValue<NetworkItemDescriptor>();
        });
        uuid = stream.readUUID().value_or("");
        blockName = stream.readString().value_or("");
        priority = stream.readVarInt().value_or(0);
        symmetric = stream.readBool().value_or(false);
        unlockingRequirement = stream.readOptional([&]() {
            return stream.readObject<RecipeUnlockingRequirement>();
        });
        recipeNetId = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(recipeId);
        if (input.empty() || input.size() > 3) {
            throw std::invalid_argument("Invalid shaped recipe height");
        }
        const auto width = input.front().size();
        stream.writeVarInt(static_cast<int32_t>(width));
        stream.writeVarInt(static_cast<int32_t>(input.size()));
        stream.writeUnsignedVarInt(static_cast<uint32_t>(width * input.size()));
        for (const auto& row : input) {
            if (row.size() != width) {
                throw std::invalid_argument("Inconsistent shaped recipe row width");
            }
            for (const auto& ingredient : row) {
                ingredient->write(stream);
            }
        }
        stream.writeList(output, [&](const auto& item) {
            item.write(stream);
        });
        stream.writeUUID(uuid);
        stream.writeString(blockName);
        stream.writeVarInt(priority);
        stream.writeBool(symmetric);
        stream.writeOptional(unlockingRequirement, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeUnsignedVarInt(recipeNetId);
    }
};

} // namespace bedrock::protocol
