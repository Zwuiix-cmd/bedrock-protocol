#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace bedrock::protocol {


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

    static std::shared_ptr<CameraAimAssistPresetExclusionDefinition> create(ReadOnlyBinaryStream& stream) {
		auto blocks = std::vector<std::string>();
		auto blocksCount = stream.readUnsignedVarInt().value_or(0);
		const bool blocksCountAvailable = stream.canReadCollection(blocksCount);
		for (std::size_t i = 0; i < blocksCount && blocksCountAvailable && !stream.hasOverflowed(); ++i) {
		    blocks.push_back(stream.readString().value_or(""));
		}
		auto entities = std::vector<std::string>();
		auto entitiesCount = stream.readUnsignedVarInt().value_or(0);
		const bool entitiesCountAvailable = stream.canReadCollection(entitiesCount);
		for (std::size_t i = 0; i < entitiesCount && entitiesCountAvailable && !stream.hasOverflowed(); ++i) {
		    entities.push_back(stream.readString().value_or(""));
		}
		auto blockTags = std::vector<std::string>();
		auto blockTagsCount = stream.readUnsignedVarInt().value_or(0);
		const bool blockTagsCountAvailable = stream.canReadCollection(blockTagsCount);
		for (std::size_t i = 0; i < blockTagsCount && blockTagsCountAvailable && !stream.hasOverflowed(); ++i) {
		    blockTags.push_back(stream.readString().value_or(""));
		}
		auto entityTypeFamilies = std::vector<std::string>();
		auto entityTypeFamiliesCount = stream.readUnsignedVarInt().value_or(0);
		const bool entityTypeFamiliesCountAvailable = stream.canReadCollection(entityTypeFamiliesCount);
		for (std::size_t i = 0; i < entityTypeFamiliesCount && entityTypeFamiliesCountAvailable && !stream.hasOverflowed(); ++i) {
		    entityTypeFamilies.push_back(stream.readString().value_or(""));
		}
		return std::make_shared<CameraAimAssistPresetExclusionDefinition>(blocks, entities, blockTags, entityTypeFamilies);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
