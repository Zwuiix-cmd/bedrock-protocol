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
#include <bedrock/protocol/types/ChunkPosition.hpp>
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
#include <bedrock/protocol/types/ChunkPosition.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class LevelChunkPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::LevelChunk;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr int64_t MAX_BLOB_HASHES = 64;

    static constexpr auto CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT = UINT32_MAX;
    static constexpr auto CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT = (UINT32_MAX - 1);

    std::shared_ptr<ChunkPosition> chunkPosition;
    int32_t dimensionId;
    uint32_t subChunkCount;
    bool clientSubChunkRequestsEnabled;
    std::optional<std::vector<uint64_t>> usedBlobHashes;
    std::string extraPayload;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		chunkPosition = std::make_shared<ChunkPosition>();
		chunkPosition->read(stream);
		dimensionId = stream.readVarInt().value_or(0);
		auto subChunkCountButNotReally = stream.readUnsignedVarInt().value_or(0);
		if (subChunkCountButNotReally == CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT) {
			clientSubChunkRequestsEnabled = true;
			subChunkCount = std::numeric_limits<uint32_t>::max();
		} else if (subChunkCountButNotReally == CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT) {
			clientSubChunkRequestsEnabled = true;
			subChunkCount = stream.readUnsignedShort().value_or(0);
		} else {
			clientSubChunkRequestsEnabled = false;
			subChunkCount = subChunkCountButNotReally;
		}
		auto cacheEnabled = stream.readBool().value_or(false);
		if (cacheEnabled) {
			usedBlobHashes.emplace();
			auto count = stream.readUnsignedVarInt().value_or(0);
			if (count > MAX_BLOB_HASHES) {
				throw std::exception{};
			}
			    const bool countAvailable = stream.canReadCollection(count);
			for (std::size_t i = 0; (i < count) && !stream.hasOverflowed() && countAvailable && !stream.hasOverflowed(); ++i) {
				usedBlobHashes.value().push_back(stream.readUnsignedLong().value_or(0));
			}
		}
		extraPayload = stream.readString().value_or("");
    }

    void write(BinaryStream& stream) override {
		chunkPosition->write(stream);
		stream.writeVarInt(dimensionId);
		if (clientSubChunkRequestsEnabled) {
			if (subChunkCount == std::numeric_limits<uint32_t>::max()) {
				stream.writeUnsignedVarInt(CLIENT_REQUEST_FULL_COLUMN_FAKE_COUNT);
			} else {
				stream.writeUnsignedVarInt(CLIENT_REQUEST_TRUNCATED_COLUMN_FAKE_COUNT);
				stream.writeUnsignedShort(subChunkCount);
			}
		} else {
			stream.writeUnsignedVarInt(subChunkCount);
		}
		stream.writeBool(usedBlobHashes.has_value());
		if (usedBlobHashes.has_value()) {
			stream.writeUnsignedVarInt(usedBlobHashes.value().size());
			for (const auto& hash : usedBlobHashes.value()) {
				stream.writeUnsignedLong(hash);
			}
		}
		stream.writeString(extraPayload);
    }
};
