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
#include <bedrock/protocol/types/camera/CameraAimAssistCategoryPriority.hpp>

class CameraAimAssistCategoryPriorities {
public:
    std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>> entities;
    std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>> blocks;
    std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>> blockTags;
    std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>> entityTypeFamilies;
    std::optional<int32_t> defaultEntityPriority;
    std::optional<int32_t> defaultBlockPriority;

    CameraAimAssistCategoryPriorities() = default;
    explicit CameraAimAssistCategoryPriorities(
        const std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>>& entitiesValue,
        const std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>>& blocksValue,
        const std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>>& blockTagsValue,
        const std::vector<std::shared_ptr<CameraAimAssistCategoryPriority>>& entityTypeFamiliesValue,
        const std::optional<int32_t>& defaultEntityPriorityValue,
        const std::optional<int32_t>& defaultBlockPriorityValue
    ) :
        entities(entitiesValue),
        blocks(blocksValue),
        blockTags(blockTagsValue),
        entityTypeFamilies(entityTypeFamiliesValue),
        defaultEntityPriority(defaultEntityPriorityValue),
        defaultBlockPriority(defaultBlockPriorityValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        entities = stream.readObjectList<CameraAimAssistCategoryPriority>();
        blocks = stream.readObjectList<CameraAimAssistCategoryPriority>();
        blockTags = stream.readObjectList<CameraAimAssistCategoryPriority>();
        entityTypeFamilies = stream.readObjectList<CameraAimAssistCategoryPriority>();
        defaultEntityPriority = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
        defaultBlockPriority = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(entities.size());
		for (const auto& entity : entities) {
			entity->write(stream);
		}
		stream.writeUnsignedVarInt(blocks.size());
		for (const auto& block : blocks) {
			block->write(stream);
		}
		stream.writeUnsignedVarInt(blockTags.size());
		for (const auto& tag : blockTags) {
			tag->write(stream);
		}
		stream.writeUnsignedVarInt(entityTypeFamilies.size());
		for (const auto& family : entityTypeFamilies) {
			family->write(stream);
		}
		stream.writeOptional(defaultEntityPriority, [&](const auto& value) {
			stream.writeInt(value);
		});
		stream.writeOptional(defaultBlockPriority, [&](const auto& value) {
			stream.writeInt(value);
		});
    }
};
