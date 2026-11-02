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
#include <bedrock/protocol/types/InputMode.hpp>
#include <bedrock/protocol/types/PlayMode.hpp>
#include <bedrock/protocol/types/InteractionMode.hpp>
#include <bedrock/protocol/types/ItemInteractionData.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequest.hpp>
#include <bedrock/protocol/types/PlayerAuthInputVehicleInfo.hpp>
#include <bedrock/protocol/types/ActorEvent.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentPayloadType.hpp>
#include <bedrock/protocol/types/AttributeLayerSettingsWeightType.hpp>
#include <bedrock/protocol/types/AttributeLayerSyncType.hpp>
#include <bedrock/protocol/types/entity/AttributeModifierOperation.hpp>
#include <bedrock/protocol/types/entity/AttributeModifierTargetOperand.hpp>
#include <bedrock/protocol/types/AttributeValueColorType.hpp>
#include <bedrock/protocol/types/AttributeValueType.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistPresetsPacketOperation.hpp>
#include <bedrock/protocol/types/ChatRestrictionLevel.hpp>
#include <bedrock/protocol/types/command/CommandParameterTypes.hpp>
#include <bedrock/protocol/types/inventory/ContainerIds.hpp>
#include <bedrock/protocol/types/inventory/ContainerUIIds.hpp>
#include <bedrock/protocol/types/ddui/DataStoreOperationType.hpp>
#include <bedrock/protocol/types/ddui/update/DataStoreUpdateValueType.hpp>
#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/DimensionIds.hpp>
#include <bedrock/protocol/types/cereal/DynamicValueType.hpp>
#include <bedrock/protocol/types/EditorLevelMigrationVersion.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationEditionOffer.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
#include <bedrock/protocol/types/GameRuleType.hpp>
#include <bedrock/protocol/types/GeneratorType.hpp>
#include <bedrock/protocol/types/LevelEvent.hpp>
#include <bedrock/protocol/types/MemoryCategory.hpp>
#include <bedrock/protocol/types/MultiplayerGameVisibility.hpp>
#include <bedrock/protocol/types/ParticleIds.hpp>
#include <bedrock/protocol/types/PlayerAction.hpp>
#include <bedrock/protocol/types/entity/PlayerMetadataFlags.hpp>
#include <bedrock/protocol/types/SubChunkPacketHeightMapType.hpp>
#include <bedrock/protocol/types/SubChunkRequestResult.hpp>
#include <bedrock/protocol/types/SyncWorldClocksType.hpp>
#include <bedrock/protocol/types/UIProfile.hpp>
#include <bedrock/protocol/types/inventory/WindowTypes.hpp>
#include <bedrock/protocol/types/AgentActionType.hpp>
#include <bedrock/protocol/types/ArmorSlot.hpp>
#include <bedrock/protocol/types/sound/AudioContentPlaybackType.hpp>
#include <bedrock/protocol/types/login/AuthenticationType.hpp>
#include <bedrock/protocol/types/BossBarColor.hpp>
#include <bedrock/protocol/types/BossBarOverlay.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistActionType.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistTargetMode.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/command/CommandPermissions.hpp>
#include <bedrock/protocol/types/CompressionAlgorithm.hpp>
#include <bedrock/protocol/types/ControlScheme.hpp>
#include <bedrock/protocol/types/inventory/CursorItemDragState.hpp>
#include <bedrock/protocol/types/EmoteType.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLayout.hpp>
#include <bedrock/protocol/types/furnace/FurnaceLeftTab.hpp>
#include <bedrock/protocol/types/furnace/FurnaceType.hpp>
#include <bedrock/protocol/types/GameMode.hpp>
#include <bedrock/protocol/types/GraphicsMode.hpp>
#include <bedrock/protocol/types/GraphicsOverrideParameterType.hpp>
#include <bedrock/protocol/types/inventory/HandSlot.hpp>
#include <bedrock/protocol/types/hud/HudElement.hpp>
#include <bedrock/protocol/types/hud/HudVisibility.hpp>
#include <bedrock/protocol/types/inventory/InventoryLayout.hpp>
#include <bedrock/protocol/types/inventory/InventoryLeftTab.hpp>
#include <bedrock/protocol/types/inventory/InventoryRightTab.hpp>
#include <bedrock/protocol/types/recipe/ItemDescriptorType.hpp>
#include <bedrock/protocol/types/hud/LoadingScreenType.hpp>
#include <bedrock/protocol/types/MatchmakingState.hpp>
#include <bedrock/protocol/types/MovementEffectType.hpp>
#include <bedrock/protocol/types/NoiseAlignmentType.hpp>
#include <bedrock/protocol/types/OverrideUpdateType.hpp>
#include <bedrock/protocol/types/PackSettingType.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>
#include <bedrock/protocol/types/PlayerLocationType.hpp>
#include <bedrock/protocol/types/PlayerPermissions.hpp>
#include <bedrock/protocol/types/inventory/PredictedResult.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeType.hpp>
#include <bedrock/protocol/types/recipe/RecipeUnlockingContext.hpp>
#include <bedrock/protocol/types/ResourcePackClientResponseStatus.hpp>
#include <bedrock/protocol/types/resourcepacks/ResourcePackType.hpp>
#include <bedrock/protocol/types/ScorePacketEntryAction.hpp>
#include <bedrock/protocol/types/ShowStoreOfferRedirectType.hpp>
#include <bedrock/protocol/types/skin/SkinArmSizeType.hpp>
#include <bedrock/protocol/types/sound/SoundDataEventType.hpp>
#include <bedrock/protocol/types/TextureShiftAction.hpp>
#include <bedrock/protocol/types/inventory/TriggerType.hpp>
#include <bedrock/protocol/types/InputMode.hpp>
#include <bedrock/protocol/types/InteractionMode.hpp>
#include <bedrock/protocol/types/ItemInteractionData.hpp>
#include <bedrock/protocol/types/PlayMode.hpp>
#include <bedrock/protocol/types/PlayerAuthInputFlags.hpp>
#include <bedrock/protocol/types/PlayerAuthInputVehicleInfo.hpp>
#include <bedrock/protocol/types/PlayerBlockAction.hpp>
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequest.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class PlayerAuthInputPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::PlayerAuthInput;

    PacketIds getId() override {
        return ID;
    }

public:
    glm::vec3 position;
    float pitch;
    float yaw;
    float headYaw;
    float moveVecX;
    float moveVecZ;
    PlayerAuthInputFlags inputFlags;
    InputMode inputMode;
    PlayMode playMode;
    InteractionMode interactionMode;
    glm::vec2 interactRotation;
    uint64_t tick;
    glm::vec3 delta;
    std::optional<std::shared_ptr<ItemInteractionData>> itemInteractionData;
    std::optional<std::shared_ptr<ItemStackRequest>> itemStackRequest;
    std::optional<std::vector<PlayerBlockAction>> blockActions;
    std::optional<PlayerAuthInputVehicleInfo> vehicleInfo;
    float analogMoveVecX;
    float analogMoveVecZ;
    glm::vec3 cameraOrientation;
    glm::vec2 rawMove;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		pitch = stream.readFloat().value_or(0.0f);
		yaw = stream.readFloat().value_or(0.0f);
		position = stream.readVec3().value_or(glm::vec3(0.0f));
		moveVecX = stream.readFloat().value_or(0.0f);
		moveVecZ = stream.readFloat().value_or(0.0f);
		headYaw = stream.readFloat().value_or(0.0f);
		inputFlags = PlayerAuthInputFlags{PlayerAuthInputFlags::Count};
		for (const auto& flag : stream.readList([&]() {
	return stream.readVarInt().value_or(0);
})) {
			if (inputFlags.get(flag)) {
				throw std::exception{};
			}
			inputFlags.set(flag, true);
		}
		inputMode = magic_enum::enum_cast<InputMode>(stream.readUnsignedVarInt().value_or(0)).value_or(InputMode::Undefined);
		playMode = magic_enum::enum_cast<PlayMode>(stream.readUnsignedVarInt().value_or(0)).value_or(PlayMode::Normal);
		interactionMode = magic_enum::enum_cast<InteractionMode>(stream.readVarInt().value_or(0)).value_or(InteractionMode::Touch);
		interactRotation = stream.readVec2().value_or(glm::vec2(0.0f));
		tick = stream.readUnsignedVarLong().value_or(0);
		delta = stream.readVec3().value_or(glm::vec3(0.0f));
		itemInteractionData = stream.readOptional([&]() { return ([&]() {
		    auto value = std::make_shared<ItemInteractionData>();
		    value->read(stream);
		    return value;
		}()); });
		itemStackRequest = stream.readOptional([&]() { return ([&]() {
		    auto value = std::make_shared<ItemStackRequest>();
		    value->read(stream);
		    return value;
		}()); });
		blockActions = stream.readOptional([&]() {
			return stream.readList(([&]() { PlayerBlockAction value; value.read(stream); return value; }()));
		});
		auto vehicleRotation = stream.readOptional([&]() {
			return stream.readVec2().value_or(glm::vec2(0.0f));
		});
		auto vehicleActorUniqueId = stream.readOptional([&]() {
			return stream.readActorUniqueID().value_or(0);
		});
		if ((vehicleRotation.has_value() && vehicleActorUniqueId.has_value())) {
			vehicleInfo = PlayerAuthInputVehicleInfo{*(vehicleRotation), *(vehicleActorUniqueId)};
		} else if (((!vehicleRotation) && (!vehicleActorUniqueId))) {
			vehicleInfo = nullptr;
		} else {
			throw std::exception{};
		}
		analogMoveVecX = stream.readFloat().value_or(0.0f);
		analogMoveVecZ = stream.readFloat().value_or(0.0f);
		cameraOrientation = stream.readVec3().value_or(glm::vec3(0.0f));
		rawMove = stream.readVec2().value_or(glm::vec2(0.0f));
    }

    void write(BinaryStream& stream) override {
		stream.writeFloat(pitch);
		stream.writeFloat(yaw);
		stream.writeVec3(position);
		stream.writeFloat(moveVecX);
		stream.writeFloat(moveVecZ);
		stream.writeFloat(headYaw);
		auto flagsArray = std::vector<int32_t>();
		for (std::size_t i = 0; (i < magic_enum::enum_integer(PlayerAuthInputFlags::Count)); ++i) {
			if (inputFlags.get(i)) {
				flagsArray.push_back(i);
			}
		}
		stream.writeList(flagsArray, [&](const auto& value) {
			stream.writeVarInt(value);
		});
		stream.writeUnsignedVarInt(magic_enum::enum_integer(inputMode));
		stream.writeUnsignedVarInt(magic_enum::enum_integer(playMode));
		stream.writeVarInt(magic_enum::enum_integer(interactionMode));
		stream.writeVec2(interactRotation);
		stream.writeUnsignedVarLong(tick);
		stream.writeVec3(delta);
		stream.writeOptional(itemInteractionData, [&](const auto& data) {
			data->write(stream);
		});
		stream.writeOptional(itemStackRequest, [&](const auto& request) {
			request->write(stream);
		});
		stream.writeOptional(blockActions, [&](const auto& array) {
			stream.writeList(array, [&](const auto& v) {
			v.write(stream);
			});
		});
		stream.writeOptional(vehicleInfo ? std::optional{vehicleInfo->vehicleRotation} : std::nullopt, [&](const auto& value) {
			stream.writeVec2(value);
		});
		stream.writeOptional(vehicleInfo ? std::optional{vehicleInfo->predictedVehicleActorUniqueId} : std::nullopt, [&](const auto& value) {
			stream.writeActorUniqueID(value);
		});
		stream.writeFloat(analogMoveVecX);
		stream.writeFloat(analogMoveVecZ);
		stream.writeVec3(cameraOrientation);
		stream.writeVec2(rawMove);
    }
};
