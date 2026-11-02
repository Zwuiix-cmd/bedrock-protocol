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
#include <bedrock/protocol/types/camera/CameraSetInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraFadeInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraTargetInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraFovInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraSplineInstruction.hpp>
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
#include <bedrock/protocol/types/camera/CameraFadeInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraFovInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraSplineInstruction.hpp>
#include <bedrock/protocol/types/camera/CameraTargetInstruction.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class CameraInstructionPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::CameraInstruction;

    PacketIds getId() override {
        return ID;
    }

public:
    std::optional<std::shared_ptr<CameraSetInstruction>> set;
    std::optional<bool> clear;
    std::optional<std::shared_ptr<CameraFadeInstruction>> fade;
    std::optional<std::shared_ptr<CameraTargetInstruction>> target;
    std::optional<bool> removeTarget;
    std::optional<std::shared_ptr<CameraFovInstruction>> fieldOfView;
    std::optional<std::shared_ptr<CameraSplineInstruction>> spline;
    std::optional<int32_t> attachToEntity;
    std::optional<bool> detachFromEntity;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		set = std::make_shared<CameraSetInstruction>();
		set.value()->read(stream);
		clear = stream.readOptional([&]() {
			return stream.readBool().value_or(false);
		});
		fade = std::make_shared<CameraFadeInstruction>();
		fade.value()->read(stream);
		target = std::make_shared<CameraTargetInstruction>();
		target.value()->read(stream);
		removeTarget = stream.readOptional([&]() {
			return stream.readBool().value_or(false);
		});
		fieldOfView = std::make_shared<CameraFovInstruction>();
		fieldOfView.value()->read(stream);
		spline = std::make_shared<CameraSplineInstruction>();
		spline.value()->read(stream);
		attachToEntity = stream.readOptional([&]() {
			return stream.readLong().value_or(0);
		});
		detachFromEntity = stream.readOptional([&]() {
			return stream.readBool().value_or(false);
		});
    }

    void write(BinaryStream& stream) override {
		stream.writeOptional(set, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(clear, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(fade, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(target, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(removeTarget, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(fieldOfView, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(spline, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(attachToEntity, [&](const auto& value) {
			stream.writeLong(value);
		});
		stream.writeOptional(detachFromEntity, [&](const auto& value) {
			stream.writeBool(value);
		});
    }
};
