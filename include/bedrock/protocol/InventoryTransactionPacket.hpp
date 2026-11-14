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
#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>
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
#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>
#include <bedrock/protocol/types/inventory/TransactionData.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>
namespace bedrock::protocol {


class InventoryTransactionPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::InventoryTransaction;

    PacketIds getId() override {
        return ID;
    }

public:
    enum class Type : uint8_t {
        Normal = 0,
        Mismatch = 1,
        UseItem = 2,
        UseItemOnEntity = 3,
        ReleaseItem = 4,
    };

    int32_t requestId;
    std::optional<std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>>> requestChangedSlots;
    std::shared_ptr<TransactionData> trData;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		requestId = stream.readVarInt().value_or(0);
		requestChangedSlots = stream.readOptional([&]() {
					auto result = std::vector<int32_t>();
					auto resultCount = stream.readUnsignedVarInt().value_or(0);
			const bool resultCountAvailable = stream.canReadCollection(resultCount);
			for (std::size_t i = 0; i < resultCount && resultCountAvailable && !stream.hasOverflowed(); ++i) {
			    result.push_back(stream.readObject<InventoryTransactionChangedSlotsHack>());
			}
					return result;
				});
		auto transactionType = stream.readUnsignedVarInt().value_or(0);
		switch (transactionType) {
			case NormalTransactionData::ID:
			{
				trData = std::make_shared<NormalTransactionData>();
				break;
			}
			case MismatchTransactionData::ID:
			{
				trData = std::make_shared<MismatchTransactionData>();
				break;
			}
			case UseItemTransactionData::ID:
			{
				trData = std::make_shared<UseItemTransactionData>();
				break;
			}
			case UseItemOnEntityTransactionData::ID:
			{
				trData = std::make_shared<UseItemOnEntityTransactionData>();
				break;
			}
			case ReleaseItemTransactionData::ID:
			{
				trData = std::make_shared<ReleaseItemTransactionData>();
				break;
			}
			default:
			{
				throw std::exception{};
			}
		}
		trData->read(stream);
    }

    void write(BinaryStream& stream) override {
		stream.writeVarInt(requestId);
		stream.writeOptional(requestChangedSlots, [&](const auto& value) {
					stream.writeUnsignedVarInt(value.size());
					for (const auto& changedSlots : value) {
				changedSlots->write(stream);
			}
				});
		stream.writeUnsignedVarInt(trData->getTypeId());
		trData->write(stream);
    }
};

} // namespace bedrock::protocol
