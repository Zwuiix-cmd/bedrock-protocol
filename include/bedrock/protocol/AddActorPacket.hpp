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
#include <bedrock/protocol/types/entity/Attribute.hpp>
#include <bedrock/protocol/types/entity/EntityLink.hpp>
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
#include <bedrock/protocol/types/recipe/CraftingRecipeBlockName.hpp>
#include <bedrock/protocol/types/DataStoreType.hpp>
#include <bedrock/protocol/types/DataStoreValueType.hpp>
#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/DimensionIds.hpp>
#include <bedrock/protocol/types/DimensionNameIds.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationEditionOffer.hpp>
#include <bedrock/protocol/types/entity/EntityIds.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
#include <bedrock/protocol/types/recipe/FurnaceRecipeBlockName.hpp>
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
#include <bedrock/protocol/types/ScreenCloseReason.hpp>
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
#include <bedrock/protocol/types/entity/Attribute.hpp>
#include <bedrock/protocol/types/entity/EntityLink.hpp>
#include <bedrock/protocol/types/entity/MetadataProperty.hpp>
#include <bedrock/protocol/types/entity/PropertySyncData.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class AddActorPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::AddActor;

    PacketIds getId() override {
        return ID;
    }

public:
    int64_t actorUniqueId;
    uint64_t actorRuntimeId;
    std::string type;
    glm::vec3 position;
    std::optional<glm::vec3> motion;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float headYaw = 0.0f;
    float bodyYaw = 0.0f;
    std::vector<std::shared_ptr<Attribute>> attributes;
    EntityMetadataCollection metadata;
    PropertySyncData syncedProperties;
    std::vector<std::shared_ptr<EntityLink>> links;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		actorUniqueId = stream.readActorUniqueID().value_or(0);
		actorRuntimeId = stream.readActorRuntimeID().value_or(0);
		type = stream.readString().value_or("");
		position = stream.readVec3().value_or(glm::vec3(0.0f));
		motion = stream.readVec3().value_or(glm::vec3(0.0f));
		pitch = stream.readFloat().value_or(0.0f);
		yaw = stream.readFloat().value_or(0.0f);
		headYaw = stream.readFloat().value_or(0.0f);
		bodyYaw = stream.readFloat().value_or(0.0f);
		auto attrCount = stream.readUnsignedVarInt().value_or(0);
		if (attrCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; (i < attrCount); ++i) {
			auto id = stream.readString().value_or("");
			auto min = stream.readFloat().value_or(0.0f);
			auto current = stream.readFloat().value_or(0.0f);
			auto max = stream.readFloat().value_or(0.0f);
			attributes.push_back(std::make_shared<Attribute>(id, min, max, current, current, std::vector<std::shared_ptr<AttributeModifier>>{}));
		}
		metadata.read(stream);
		syncedProperties = stream.readValue<PropertySyncData>();
		auto linkCount = stream.readUnsignedVarInt().value_or(0);
		if (linkCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; (i < linkCount); ++i) {
			links.push_back(stream.readObject<EntityLink>());
		}
    }

    void write(BinaryStream& stream) override {
		stream.writeActorUniqueID(actorUniqueId);
		stream.writeActorRuntimeID(actorRuntimeId);
		stream.writeString(type);
		stream.writeVec3(position);
		stream.writeVec3(motion.value_or(glm::vec3(0.0f)));
		stream.writeFloat(pitch);
		stream.writeFloat(yaw);
		stream.writeFloat(headYaw);
		stream.writeFloat(bodyYaw);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			stream.writeString(attribute->id);
			stream.writeFloat(attribute->min);
			stream.writeFloat(attribute->current);
			stream.writeFloat(attribute->max);
		}
		metadata.write(stream);
		syncedProperties.write(stream);
		stream.writeUnsignedVarInt(links.size());
		for (const auto& link : links) {
			if (link) link->write(stream);
		}
    }
};
