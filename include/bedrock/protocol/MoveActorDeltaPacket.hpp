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
#include <bedrock/protocol/types/recipe/CraftingRecipeBlockName.hpp>
#include <bedrock/protocol/types/ddui/DataStoreOperationType.hpp>
#include <bedrock/protocol/types/ddui/update/DataStoreUpdateValueType.hpp>
#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/DimensionIds.hpp>
#include <bedrock/protocol/types/DimensionNameIds.hpp>
#include <bedrock/protocol/types/cereal/DynamicValueType.hpp>
#include <bedrock/protocol/types/EditorLevelMigrationVersion.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationEditionOffer.hpp>
#include <bedrock/protocol/types/entity/EntityIds.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
#include <bedrock/protocol/types/recipe/FurnaceRecipeBlockName.hpp>
#include <bedrock/protocol/types/GameRuleType.hpp>
#include <bedrock/protocol/types/GeneratorType.hpp>
#include <bedrock/protocol/types/LevelEvent.hpp>
#include <bedrock/protocol/types/LevelSoundEvent.hpp>
#include <bedrock/protocol/types/MemoryCategory.hpp>
#include <bedrock/protocol/types/MultiplayerGameVisibility.hpp>
#include <bedrock/protocol/types/ParticleIds.hpp>
#include <bedrock/protocol/types/PlayerAction.hpp>
#include <bedrock/protocol/types/entity/PlayerMetadataFlags.hpp>
#include <bedrock/protocol/types/ScreenCloseReason.hpp>
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
#include <bedrock/protocol/types/InputMode.hpp>
#include <bedrock/protocol/types/InteractionMode.hpp>
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
#include <bedrock/protocol/types/PlayMode.hpp>
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
#include <bedrock/protocol/types/TypesGenerated.hpp>

class MoveActorDeltaPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::MoveActorDelta;

    PacketIds getId() override {
        return ID;
    }

public:
    uint64_t actorRuntimeId;
    std::optional<float> xPos;
    std::optional<float> yPos;
    std::optional<float> zPos;
    std::optional<float> pitch;
    std::optional<float> yaw;
    std::optional<float> headYaw;
    bool onGround = false;
    bool teleport = false;
    bool forceMoveLocalEntity = false;
    bool forceCompletion = false;
    uint64_t ticks;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		actorRuntimeId = stream.readActorRuntimeID().value_or(0);
		xPos = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readFloat().value_or(0.0f);
			});
		}());
		yPos = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readFloat().value_or(0.0f);
			});
		}());
		zPos = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readFloat().value_or(0.0f);
			});
		}());
		pitch = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readRotationByte().value_or(0);
			});
		}());
		yaw = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readRotationByte().value_or(0);
			});
		}());
		headYaw = ([&]() -> std::optional<float> {
			return stream.readOptional([&]() {
				return stream.readRotationByte().value_or(0);
			});
		}());
		onGround = stream.readBool().value_or(false);
		teleport = stream.readBool().value_or(false);
		forceMoveLocalEntity = stream.readBool().value_or(false);
		forceCompletion = stream.readBool().value_or(false);
		ticks = stream.readUnsignedVarLong().value_or(0);
    }

    void write(BinaryStream& stream) override {
		stream.writeActorRuntimeID(actorRuntimeId);
		([&]() {
			stream.writeOptional(xPos, [&](const auto& value) {
				stream.writeFloat(value);
			});
		}());
		([&]() {
			stream.writeOptional(yPos, [&](const auto& value) {
				stream.writeFloat(value);
			});
		}());
		([&]() {
			stream.writeOptional(zPos, [&](const auto& value) {
				stream.writeFloat(value);
			});
		}());
		([&]() {
			stream.writeOptional(pitch, [&](const auto& value) {
				stream.writeRotationByte(value);
			});
		}());
		([&]() {
			stream.writeOptional(yaw, [&](const auto& value) {
				stream.writeRotationByte(value);
			});
		}());
		([&]() {
			stream.writeOptional(headYaw, [&](const auto& value) {
				stream.writeRotationByte(value);
			});
		}());
		stream.writeBool(onGround);
		stream.writeBool(teleport);
		stream.writeBool(forceMoveLocalEntity);
		stream.writeBool(forceCompletion);
		stream.writeUnsignedVarLong(ticks);
    }
};
