#pragma once

#include <cstdint>
#include <exception>
#include <limits>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <variant>
#include <vector>
#include <bedrock/protocol/Packet.hpp>
#include <bedrock/protocol/types/MapTrackedObject.hpp>
#include <bedrock/protocol/types/MapDecoration.hpp>
#include <bedrock/protocol/types/MapImage.hpp>
#include <bedrock/protocol/types/ActorEvent.hpp>
#include <bedrock/protocol/types/AgentActionType.hpp>
#include <bedrock/protocol/types/AttributeLayerSettingsWeightType.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncType.hpp>
#include <bedrock/protocol/types/entity/AttributeModifierOperation.hpp>
#include <bedrock/protocol/types/entity/AttributeModifierTargetOperand.hpp>
#include <bedrock/protocol/types/AttributeValueColorType.hpp>
#include <bedrock/protocol/types/AttributeValueType.hpp>
#include <bedrock/protocol/types/BossBarColor.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetsPacketOperation.hpp>
#include <bedrock/protocol/types/ChatRestrictionLevel.hpp>
#include <bedrock/protocol/types/command/CommandParameterTypes.hpp>
#include <bedrock/protocol/types/CompressionAlgorithm.hpp>
#include <bedrock/protocol/types/inventory/ContainerIds.hpp>
#include <bedrock/protocol/types/inventory/ContainerUIIds.hpp>
#include <bedrock/protocol/types/DataStoreType.hpp>
#include <bedrock/protocol/types/DataStoreValueType.hpp>
#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/DimensionIds.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationEditionOffer.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
#include <bedrock/protocol/types/GameMode.hpp>
#include <bedrock/protocol/types/GameRuleType.hpp>
#include <bedrock/protocol/types/GeneratorType.hpp>
#include <bedrock/protocol/types/InputMode.hpp>
#include <bedrock/protocol/types/InteractionMode.hpp>
#include <bedrock/protocol/types/recipe/ItemDescriptorType.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestActionType.hpp>
#include <bedrock/protocol/types/LevelEvent.hpp>
#include <bedrock/protocol/types/LevelSoundEvent.hpp>
#include <bedrock/protocol/types/MemoryCategory.hpp>
#include <bedrock/protocol/types/MultiplayerGameVisibility.hpp>
#include <bedrock/protocol/types/ParticleIds.hpp>
#include <bedrock/protocol/types/PlayMode.hpp>
#include <bedrock/protocol/types/PlayerAction.hpp>
#include <bedrock/protocol/types/entity/PlayerMetadataFlags.hpp>
#include <bedrock/protocol/types/PlayerPermissions.hpp>
#include <bedrock/protocol/types/resourcepacks/ResourcePackType.hpp>
#include <bedrock/protocol/types/SubChunkPacketHeightMapType.hpp>
#include <bedrock/protocol/types/SubChunkRequestResult.hpp>
#include <bedrock/protocol/types/SyncWorldClocksType.hpp>
#include <bedrock/protocol/types/TextureShiftAction.hpp>
#include <bedrock/protocol/types/UIProfile.hpp>
#include <bedrock/protocol/types/inventory/WindowTypes.hpp>
#include <bedrock/protocol/types/ArmorSlot.hpp>
#include <bedrock/protocol/types/login/AuthenticationType.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistActionType.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistTargetMode.hpp>
#include <bedrock/protocol/types/ControlScheme.hpp>
#include <bedrock/protocol/types/GraphicsMode.hpp>
#include <bedrock/protocol/types/GraphicsOverrideParameterType.hpp>
#include <bedrock/protocol/types/hud/HudElement.hpp>
#include <bedrock/protocol/types/hud/HudVisibility.hpp>
#include <bedrock/protocol/types/inventory/InventoryLayout.hpp>
#include <bedrock/protocol/types/inventory/InventoryLeftTab.hpp>
#include <bedrock/protocol/types/inventory/InventoryRightTab.hpp>
#include <bedrock/protocol/types/hud/LoadingScreenType.hpp>
#include <bedrock/protocol/types/MovementEffectType.hpp>
#include <bedrock/protocol/types/OverrideUpdateType.hpp>
#include <bedrock/protocol/types/PackSettingType.hpp>
#include <bedrock/protocol/types/PlayerLocationType.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/ScriptDebugShapeType.hpp>
#include <bedrock/protocol/types/ShowStoreOfferRedirectType.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
#include <bedrock/protocol/types/MapDecoration.hpp>
#include <bedrock/protocol/types/MapImage.hpp>
#include <bedrock/protocol/types/MapTrackedObject.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class ClientboundMapItemDataPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::ClientboundMapItemData;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr int64_t BITFLAG_TEXTURE_UPDATE = 2;
    static constexpr int64_t BITFLAG_DECORATION_UPDATE = 4;
    static constexpr int64_t BITFLAG_MAP_CREATION = 8;

    int64_t mapId;
    uint32_t type;
    uint8_t dimensionId = 0;
    bool isLocked = false;
    glm::ivec3 origin;
    std::vector<int64_t> parentMapIds;
    uint8_t scale;
    std::vector<std::shared_ptr<MapTrackedObject>> trackedEntities;
    std::vector<std::shared_ptr<MapDecoration>> decorations;
    int32_t xOffset = 0;
    int32_t yOffset = 0;
    std::optional<std::shared_ptr<MapImage>> colors;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		mapId = stream.readActorUniqueID().value_or(0);
		type = stream.readUnsignedVarInt().value_or(0);
		dimensionId = stream.readUnsignedByte().value_or(0);
		isLocked = stream.readBool().value_or(false);
		origin = stream.readSignedBlockPos().value_or(glm::ivec3(0));
		if (((type & BITFLAG_MAP_CREATION) != 0)) {
			auto count = stream.readUnsignedVarInt().value_or(0);
			for (std::size_t i = 0; (i < count); ++i) {
				parentMapIds.push_back(stream.readActorUniqueID().value_or(0));
			}
		}
		if (((type & ((BITFLAG_MAP_CREATION | BITFLAG_DECORATION_UPDATE) | BITFLAG_TEXTURE_UPDATE)) != 0)) {
			scale = stream.readUnsignedByte().value_or(0);
		}
		if (((type & BITFLAG_DECORATION_UPDATE) != 0)) {
			{
			    auto count = stream.readUnsignedVarInt().value_or(0);
			    for (std::size_t i = 0; (i < count); ++i) {
					auto object = std::make_shared<MapTrackedObject>();
					object->type = static_cast<MapTrackedObject::Type>(stream.readUnsignedInt().value_or(0));
					if ((static_cast<std::underlying_type_t<decltype(object->type)>>(object->type) == static_cast<std::underlying_type_t<decltype(MapTrackedObject::Type::Block)>>(MapTrackedObject::Type::Block))) {
						object->blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
					} else if ((static_cast<std::underlying_type_t<decltype(object->type)>>(object->type) == static_cast<std::underlying_type_t<decltype(MapTrackedObject::Type::Entity)>>(MapTrackedObject::Type::Entity))) {
						object->actorUniqueId = stream.readActorUniqueID().value_or(0);
					} else {
						throw std::exception{};
					}
					trackedEntities.push_back(object);
			    }
			}
			{
			    auto count = stream.readUnsignedVarInt().value_or(0);
			    for (std::size_t i = 0; (i < count); ++i) {
					auto icon = stream.readUnsignedByte().value_or(0);
					auto rotation = stream.readUnsignedByte().value_or(0);
					auto xOffsetValue = stream.readUnsignedByte().value_or(0);
					auto yOffsetValue = stream.readUnsignedByte().value_or(0);
					auto label = stream.readString().value_or("");
					auto color = static_cast<uint32_t>(BinaryStream::flipIntEndianness(stream.readUnsignedVarInt().value_or(0)));
					decorations.push_back(std::make_shared<MapDecoration>(icon, rotation, xOffsetValue, yOffsetValue, label, color));
			    }
			}
		}
		if (((type & BITFLAG_TEXTURE_UPDATE) != 0)) {
			auto width = stream.readVarInt().value_or(0);
			auto height = stream.readVarInt().value_or(0);
			xOffset = stream.readVarInt().value_or(0);
			yOffset = stream.readVarInt().value_or(0);
			auto count = stream.readUnsignedVarInt().value_or(0);
			if ((count != (width * height))) {
				throw std::exception{};
			}
			colors = MapImage::decode(stream, height, width);
		}
    }

    void write(BinaryStream& stream) override {
		std::remove_cvref_t<decltype(parentMapIds.size())> parentMapIdsCount{};
		std::remove_cvref_t<decltype(decorations.size())> decorationCount{};
		stream.writeActorUniqueID(mapId);
		auto type = 0;
		if (((parentMapIdsCount = parentMapIds.size()) > 0)) {
			type = type | BITFLAG_MAP_CREATION;
		}
		if (((decorationCount = decorations.size()) > 0)) {
			type = type | BITFLAG_DECORATION_UPDATE;
		}
		if (colors.has_value()) {
			type = type | BITFLAG_TEXTURE_UPDATE;
		}
		stream.writeUnsignedVarInt(type);
		stream.writeUnsignedByte(dimensionId);
		stream.writeBool(isLocked);
		stream.writeSignedBlockPos(origin);
		if (((type & BITFLAG_MAP_CREATION) != 0)) {
			stream.writeUnsignedVarInt(parentMapIdsCount);
			for (const auto& parentMapId : parentMapIds) {
				stream.writeActorUniqueID(parentMapId);
			}
		}
		if (((type & ((BITFLAG_MAP_CREATION | BITFLAG_TEXTURE_UPDATE) | BITFLAG_DECORATION_UPDATE)) != 0)) {
			stream.writeUnsignedByte(scale);
		}
		if (((type & BITFLAG_DECORATION_UPDATE) != 0)) {
			stream.writeUnsignedVarInt(trackedEntities.size());
			for (const auto& object : trackedEntities) {
				stream.writeUnsignedInt(static_cast<int32_t>(object->type));
				if ((static_cast<std::underlying_type_t<decltype(object->type)>>(object->type) == static_cast<std::underlying_type_t<decltype(MapTrackedObject::Type::Block)>>(MapTrackedObject::Type::Block))) {
					stream.writeSignedBlockPos(object->blockPosition);
				} else if ((static_cast<std::underlying_type_t<decltype(object->type)>>(object->type) == static_cast<std::underlying_type_t<decltype(MapTrackedObject::Type::Entity)>>(MapTrackedObject::Type::Entity))) {
					stream.writeActorUniqueID(object->actorUniqueId);
				} else {
					throw std::exception{};
				}
			}
			stream.writeUnsignedVarInt(decorationCount);
			for (const auto& decoration : decorations) {
				stream.writeUnsignedByte(decoration->icon);
				stream.writeUnsignedByte(decoration->rotation);
				stream.writeUnsignedByte(decoration->xOffset);
				stream.writeUnsignedByte(decoration->yOffset);
				stream.writeString(decoration->label);
				stream.writeUnsignedVarInt(BinaryStream::flipIntEndianness(static_cast<uint32_t>(decoration->color)));
			}
		}
		if (colors.has_value()) {
			stream.writeVarInt(colors.value()->width);
			stream.writeVarInt(colors.value()->height);
			stream.writeVarInt(xOffset);
			stream.writeVarInt(yOffset);
			stream.writeUnsignedVarInt((colors.value()->width * colors.value()->height));
			colors.value()->write(stream);
		}
    }
};
