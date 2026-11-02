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

class AvailableCommandsPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::AvailableCommands;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr int64_t ARG_FLAG_VALID = 1048576;
    static constexpr int64_t ARG_TYPE_INT = 1;
    static constexpr int64_t ARG_TYPE_FLOAT = 3;
    static constexpr int64_t ARG_TYPE_VALUE = 4;
    static constexpr int64_t ARG_TYPE_WILDCARD_INT = 5;
    static constexpr int64_t ARG_TYPE_OPERATOR = 6;
    static constexpr int64_t ARG_TYPE_COMPARE_OPERATOR = 7;
    static constexpr int64_t ARG_TYPE_TARGET = 8;
    static constexpr int64_t ARG_TYPE_WILDCARD_TARGET = 10;
    static constexpr int64_t ARG_TYPE_FILEPATH = 17;
    static constexpr int64_t ARG_TYPE_FULL_INTEGER_RANGE = 23;
    static constexpr int64_t ARG_TYPE_EQUIPMENT_SLOT = 47;
    static constexpr int64_t ARG_TYPE_STRING = 56;
    static constexpr int64_t ARG_TYPE_INT_POSITION = 64;
    static constexpr int64_t ARG_TYPE_POSITION = 65;
    static constexpr int64_t ARG_TYPE_MESSAGE = 68;
    static constexpr int64_t ARG_TYPE_RAWTEXT = 70;
    static constexpr int64_t ARG_TYPE_JSON = 74;
    static constexpr int64_t ARG_TYPE_BLOCK_STATES = 84;
    static constexpr int64_t ARG_TYPE_COMMAND = 90;
    static constexpr int64_t ARG_FLAG_ENUM = 2097152;
    static constexpr int64_t ARG_FLAG_POSTFIX = 16777216;
    static constexpr int64_t ARG_FLAG_SOFT_ENUM = 67108864;

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
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    enumValues.push_back(stream.readString().value_or(""));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    chainedSubCommandValues.push_back(stream.readString().value_or(""));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    postfixes.push_back(stream.readString().value_or(""));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    enums.push_back(([&]() {
		    auto value = std::make_shared<CommandEnumRawData>();
		    value->read(stream);
		    return value;
		}()));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    chainedSubCommandData.push_back(([&]() {
		    auto value = std::make_shared<ChainedSubCommandRawData>();
		    value->read(stream);
		    return value;
		}()));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    commandData.push_back(([&]() {
		    auto value = std::make_shared<CommandRawData>();
		    value->read(stream);
		    return value;
		}()));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    softEnums.push_back(([&]() {
		    auto value = std::make_shared<CommandSoftEnum>();
		    value->read(stream);
		    return value;
		}()));
		}
		}
		{
		auto size = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; i < size; i++) {
		    enumConstraints.push_back(([&]() {
		    auto value = std::make_shared<CommandEnumConstraintRawData>();
		    value->read(stream);
		    return value;
		}()));
		}
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
