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

class CameraAimAssistPresetExclusionDefinition {
public:
    std::vector<std::string> blocks;
    std::vector<std::string> entities;
    std::vector<std::string> blockTags;
    std::vector<std::string> entityTypeFamilies;

    CameraAimAssistPresetExclusionDefinition() = default;
    explicit CameraAimAssistPresetExclusionDefinition(
        const std::vector<std::string>& blocksValue,
        const std::vector<std::string>& entitiesValue,
        const std::vector<std::string>& blockTagsValue,
        const std::vector<std::string>& entityTypeFamiliesValue
    ) :
        blocks(blocksValue),
        entities(entitiesValue),
        blockTags(blockTagsValue),
        entityTypeFamilies(entityTypeFamiliesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        blocks = stream.readList([&]() {
            return stream.readString().value_or("");
        });
        entities = stream.readList([&]() {
            return stream.readString().value_or("");
        });
        blockTags = stream.readList([&]() {
            return stream.readString().value_or("");
        });
        entityTypeFamilies = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(blocks.size());
		for (const auto& block : blocks) {
			stream.writeString(block);
		}
		stream.writeUnsignedVarInt(entities.size());
		for (const auto& entity : entities) {
			stream.writeString(entity);
		}
		stream.writeUnsignedVarInt(blockTags.size());
		for (const auto& blockTag : blockTags) {
			stream.writeString(blockTag);
		}
		stream.writeUnsignedVarInt(entityTypeFamilies.size());
		for (const auto& entityTypeFamily : entityTypeFamilies) {
			stream.writeString(entityTypeFamily);
		}
    }
};
