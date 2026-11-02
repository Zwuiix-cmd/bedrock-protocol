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
#include <bedrock/protocol/types/LevelSettings.hpp>
#include <bedrock/protocol/types/PlayerMovementSettings.hpp>
#include <bedrock/protocol/types/NetworkPermissions.hpp>
#include <bedrock/protocol/types/ServerJoinInformation.hpp>
#include <bedrock/protocol/types/ServerTelemetryData.hpp>
#include <bedrock/protocol/types/BlockPaletteEntry.hpp>
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
#include <bedrock/protocol/types/BlockPaletteEntry.hpp>
#include <bedrock/protocol/types/LevelSettings.hpp>
#include <bedrock/protocol/types/NetworkNbt.hpp>
#include <bedrock/protocol/types/NetworkPermissions.hpp>
#include <bedrock/protocol/types/PlayerMovementSettings.hpp>
#include <bedrock/protocol/types/ServerJoinInformation.hpp>
#include <bedrock/protocol/types/ServerTelemetryData.hpp>
#include <bedrock/protocol/types/entity/MetadataProperty.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class StartGamePacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::StartGame;

    PacketIds getId() override {
        return ID;
    }

public:
    int64_t actorUniqueId;
    uint64_t actorRuntimeId;
    int32_t playerGamemode;
    glm::vec3 playerPosition;
    float pitch;
    float yaw;
    std::shared_ptr<CompoundTag> playerActorProperties;
    std::shared_ptr<LevelSettings> levelSettings;
    std::string levelId;
    std::string worldName;
    std::string premiumWorldTemplateId;
    bool isTrial = false;
    std::shared_ptr<PlayerMovementSettings> playerMovementSettings;
    uint64_t currentTick = 0;
    int32_t enchantmentSeed = 0;
    std::string multiplayerCorrelationId;
    bool enableNewInventorySystem = false;
    std::string serverSoftwareVersion;
    std::string worldTemplateId;
    bool enableClientSideChunkGeneration;
    bool blockNetworkIdsAreHashes = false;
    std::shared_ptr<NetworkPermissions> networkPermissions;
    std::optional<std::shared_ptr<ServerJoinInformation>> serverJoinInformation;
    std::shared_ptr<ServerTelemetryData> serverTelemetryData;
    std::vector<std::shared_ptr<BlockPaletteEntry>> blockPalette;
    uint64_t blockPaletteChecksum;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		actorUniqueId = stream.readActorUniqueID().value_or(0);
		actorRuntimeId = stream.readActorRuntimeID().value_or(0);
		playerGamemode = stream.readVarInt().value_or(0);
		playerPosition = stream.readVec3().value_or(glm::vec3(0.0f));
		pitch = stream.readFloat().value_or(0.0f);
		yaw = stream.readFloat().value_or(0.0f);
		levelSettings = std::make_shared<LevelSettings>();
		levelSettings->read(stream);
		levelId = stream.readString().value_or("");
		worldName = stream.readString().value_or("");
		premiumWorldTemplateId = stream.readString().value_or("");
		isTrial = stream.readBool().value_or(false);
		playerMovementSettings = std::make_shared<PlayerMovementSettings>();
		playerMovementSettings->read(stream);
		currentTick = stream.readUnsignedLong().value_or(0);
		enchantmentSeed = stream.readVarInt().value_or(0);
		{
		    auto len = stream.readUnsignedVarInt().value_or(0);
		    const bool lenAvailable = stream.canReadCollection(len);
		    for (std::size_t i = 0; (i < len) && !stream.hasOverflowed() && lenAvailable && !stream.hasOverflowed(); ++i) {
				auto blockName = stream.readString().value_or("");
				auto state = NetworkNbtSerializer::read(stream);
				blockPalette.push_back(std::make_shared<BlockPaletteEntry>(blockName, state));
		    }
		}
		multiplayerCorrelationId = stream.readString().value_or("");
		enableNewInventorySystem = stream.readBool().value_or(false);
		serverSoftwareVersion = stream.readString().value_or("");
		playerActorProperties = NetworkNbtSerializer::read(stream);
		blockPaletteChecksum = stream.readUnsignedLong().value_or(0);
		worldTemplateId = stream.readUUID().value_or("");
		enableClientSideChunkGeneration = stream.readBool().value_or(false);
		blockNetworkIdsAreHashes = stream.readBool().value_or(false);
		networkPermissions = std::make_shared<NetworkPermissions>();
		networkPermissions->read(stream);
		serverJoinInformation = stream.readOptional([&]() { return stream.readObject<ServerJoinInformation>(); });
		serverTelemetryData = std::make_shared<ServerTelemetryData>();
		serverTelemetryData->read(stream);
    }

    void write(BinaryStream& stream) override {
		stream.writeActorUniqueID(actorUniqueId);
		stream.writeActorRuntimeID(actorRuntimeId);
		stream.writeVarInt(playerGamemode);
		stream.writeVec3(playerPosition);
		stream.writeFloat(pitch);
		stream.writeFloat(yaw);
		levelSettings->write(stream);
		stream.writeString(levelId);
		stream.writeString(worldName);
		stream.writeString(premiumWorldTemplateId);
		stream.writeBool(isTrial);
		playerMovementSettings->write(stream);
		stream.writeUnsignedLong(currentTick);
		stream.writeVarInt(enchantmentSeed);
		stream.writeUnsignedVarInt(blockPalette.size());
		for (const auto& entry : blockPalette) {
			stream.writeString(entry->name);
			NetworkNbtSerializer::write(stream, entry->states);
		}
		stream.writeString(multiplayerCorrelationId);
		stream.writeBool(enableNewInventorySystem);
		stream.writeString(serverSoftwareVersion);
		NetworkNbtSerializer::write(stream, playerActorProperties);
		stream.writeUnsignedLong(blockPaletteChecksum);
		stream.writeUUID(worldTemplateId);
		stream.writeBool(enableClientSideChunkGeneration);
		stream.writeBool(blockNetworkIdsAreHashes);
		networkPermissions->write(stream);
		stream.writeOptional(serverJoinInformation, [&](const auto& info) {
			info->write(stream);
		});
		serverTelemetryData->write(stream);
    }
};
