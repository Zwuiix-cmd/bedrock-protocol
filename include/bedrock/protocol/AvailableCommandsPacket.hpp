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
#include <bedrock/protocol/types/command/raw/CommandEnumRawData.hpp>
#include <bedrock/protocol/types/command/raw/ChainedSubCommandRawData.hpp>
#include <bedrock/protocol/types/command/raw/CommandRawData.hpp>
#include <bedrock/protocol/types/command/CommandSoftEnum.hpp>
#include <bedrock/protocol/types/command/raw/CommandEnumConstraintRawData.hpp>
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
#include <bedrock/protocol/types/command/CommandSoftEnum.hpp>
#include <bedrock/protocol/types/command/raw/ChainedSubCommandRawData.hpp>
#include <bedrock/protocol/types/command/raw/CommandEnumConstraintRawData.hpp>
#include <bedrock/protocol/types/command/raw/CommandEnumRawData.hpp>
#include <bedrock/protocol/types/command/raw/CommandRawData.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>
namespace bedrock::protocol {


class AvailableCommandsPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::AvailableCommands;

    PacketIds getId() override {
        return ID;
    }

public:
    enum class Arg : int64_t {
        FlagValid = 1048576,
        TypeInt = 1,
        TypeFloat = 3,
        TypeValue = 4,
        TypeWildcardInt = 5,
        TypeOperator = 6,
        TypeCompareOperator = 7,
        TypeTarget = 8,
        TypeWildcardTarget = 10,
        TypeFilepath = 17,
        TypeFullIntegerRange = 23,
        TypeEquipmentSlot = 47,
        TypeString = 56,
        TypeIntPosition = 64,
        TypePosition = 65,
        TypeMessage = 68,
        TypeRawtext = 70,
        TypeJson = 74,
        TypeBlockStates = 84,
        TypeCommand = 90,
        FlagEnum = 2097152,
        FlagPostfix = 16777216,
        FlagSoftEnum = 67108864,
    };

    std::vector<std::string> enumValues;
    std::vector<std::string> chainedSubCommandValues;
    std::vector<std::string> postfixes;
    std::vector<std::shared_ptr<CommandEnumRawData>> enums;
    std::vector<std::shared_ptr<ChainedSubCommandRawData>> chainedSubCommandData;
    std::vector<std::shared_ptr<CommandRawData>> commandData;
    std::vector<std::shared_ptr<CommandSoftEnum>> softEnums;
    std::vector<std::shared_ptr<CommandEnumConstraintRawData>> enumConstraints;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		auto enumValuesCount = stream.readUnsignedVarInt().value_or(0);
		const bool enumValuesCountAvailable = stream.canReadCollection(enumValuesCount);
		for (std::size_t i = 0; i < enumValuesCount && enumValuesCountAvailable && !stream.hasOverflowed(); i++) {
		    enumValues.push_back(stream.readString().value_or(""));
		}
		auto chainedSubCommandValuesCount = stream.readUnsignedVarInt().value_or(0);
		const bool chainedSubCommandValuesCountAvailable = stream.canReadCollection(chainedSubCommandValuesCount);
		for (std::size_t i = 0; i < chainedSubCommandValuesCount && chainedSubCommandValuesCountAvailable && !stream.hasOverflowed(); i++) {
		    chainedSubCommandValues.push_back(stream.readString().value_or(""));
		}
		auto postfixesCount = stream.readUnsignedVarInt().value_or(0);
		const bool postfixesCountAvailable = stream.canReadCollection(postfixesCount);
		for (std::size_t i = 0; i < postfixesCount && postfixesCountAvailable && !stream.hasOverflowed(); i++) {
		    postfixes.push_back(stream.readString().value_or(""));
		}
		auto enumsCount = stream.readUnsignedVarInt().value_or(0);
		const bool enumsCountAvailable = stream.canReadCollection(enumsCount);
		for (std::size_t i = 0; i < enumsCount && enumsCountAvailable && !stream.hasOverflowed(); i++) {
		    enums.push_back(stream.readObject<CommandEnumRawData>());
		}
		auto chainedSubCommandDataCount = stream.readUnsignedVarInt().value_or(0);
		const bool chainedSubCommandDataCountAvailable = stream.canReadCollection(chainedSubCommandDataCount);
		for (std::size_t i = 0; i < chainedSubCommandDataCount && chainedSubCommandDataCountAvailable && !stream.hasOverflowed(); i++) {
		    chainedSubCommandData.push_back(stream.readObject<ChainedSubCommandRawData>());
		}
		auto commandDataCount = stream.readUnsignedVarInt().value_or(0);
		const bool commandDataCountAvailable = stream.canReadCollection(commandDataCount);
		for (std::size_t i = 0; i < commandDataCount && commandDataCountAvailable && !stream.hasOverflowed(); i++) {
		    commandData.push_back(stream.readObject<CommandRawData>());
		}
		auto softEnumsCount = stream.readUnsignedVarInt().value_or(0);
		const bool softEnumsCountAvailable = stream.canReadCollection(softEnumsCount);
		for (std::size_t i = 0; i < softEnumsCount && softEnumsCountAvailable && !stream.hasOverflowed(); i++) {
		    softEnums.push_back(stream.readObject<CommandSoftEnum>());
		}
		auto enumConstraintsCount = stream.readUnsignedVarInt().value_or(0);
		const bool enumConstraintsCountAvailable = stream.canReadCollection(enumConstraintsCount);
		for (std::size_t i = 0; i < enumConstraintsCount && enumConstraintsCountAvailable && !stream.hasOverflowed(); i++) {
		    enumConstraints.push_back(stream.readObject<CommandEnumConstraintRawData>());
		}
    }

    void write(BinaryStream& stream) override {
		stream.writeUnsignedVarInt(enumValues.size());
		for (const auto& value : enumValues) {
			stream.writeString(value);
		}
		stream.writeUnsignedVarInt(chainedSubCommandValues.size());
		for (const auto& value : chainedSubCommandValues) {
			stream.writeString(value);
		}
		stream.writeUnsignedVarInt(postfixes.size());
		for (const auto& postfix : postfixes) {
			stream.writeString(postfix);
		}
		stream.writeUnsignedVarInt(enums.size());
		for (const auto& enum_ : enums) {
			enum_->write(stream);
		}
		stream.writeUnsignedVarInt(chainedSubCommandData.size());
		for (const auto& data : chainedSubCommandData) {
			data->write(stream);
		}
		stream.writeUnsignedVarInt(commandData.size());
		for (const auto& data : commandData) {
			data->write(stream);
		}
		stream.writeUnsignedVarInt(softEnums.size());
		for (const auto& softEnum : softEnums) {
			softEnum->write(stream);
		}
		stream.writeUnsignedVarInt(enumConstraints.size());
		for (const auto& constraint : enumConstraints) {
			constraint->write(stream);
		}
    }
};

} // namespace bedrock::protocol
