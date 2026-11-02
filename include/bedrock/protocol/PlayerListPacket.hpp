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
#include <bedrock/protocol/types/PlayerListEntry.hpp>
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
#include <bedrock/protocol/types/PlayerListEntry.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>
namespace bedrock::protocol {


class PlayerListPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::PlayerList;

    PacketIds getId() override {
        return ID;
    }

public:
    enum class Type : uint8_t {
        Add = 0,
        Remove = 1,
    };

    Type type;
    std::vector<std::shared_ptr<PlayerListEntry>> entries;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		type = magic_enum::enum_cast<Type>(
			stream.readUnsignedByte().value_or(0)
		).value_or(Type::Add);
		auto count = stream.readUnsignedVarInt().value_or(0);
		    const bool countAvailable = stream.canReadCollection(count);
		for (std::size_t i = 0; (i < count) && !stream.hasOverflowed() && countAvailable && !stream.hasOverflowed(); ++i) {
			auto entry = std::make_shared<PlayerListEntry>();
			if (type == Type::Add) {
				entry->uuid = stream.readUUID().value_or("");
				entry->actorUniqueId = stream.readActorUniqueID().value_or(0);
				entry->username = stream.readString().value_or("");
				entry->xboxUserId = stream.readString().value_or("");
				entry->platformChatId = stream.readString().value_or("");
				entry->buildPlatform = magic_enum::enum_cast<BuildPlatform>(
					stream.readInt().value_or(0)
				).value_or(BuildPlatform::Unknown);
				entry->skinData = std::make_shared<SkinData>();
				entry->skinData->read(stream);
				entry->isTeacher = stream.readBool().value_or(false);
				entry->isHost = stream.readBool().value_or(false);
				entry->isSubClient = stream.readBool().value_or(false);
				entry->color = static_cast<uint32_t>(stream.readUnsignedInt().value_or(0));
			} else {
				entry->uuid = stream.readUUID().value_or("");
			}
			entries.push_back(entry);
		}
		if (type == Type::Add) {
			    const bool countAvailable = stream.canReadCollection(count);
			for (std::size_t i = 0; (i < count) && !stream.hasOverflowed() && countAvailable && !stream.hasOverflowed(); ++i) {
				entries[i]->skinData->isVerified = stream.readBool().value_or(false);
			}
		}
    }

    void write(BinaryStream& stream) override {
		stream.writeUnsignedByte(magic_enum::enum_integer(type));
		stream.writeUnsignedVarInt(entries.size());
		for (const auto& entry : entries) {
			if (type == Type::Add) {
				stream.writeUUID(entry->uuid);
				stream.writeActorUniqueID(entry->actorUniqueId);
				stream.writeString(entry->username);
				stream.writeString(entry->xboxUserId);
				stream.writeString(entry->platformChatId);
				stream.writeInt(magic_enum::enum_integer(entry->buildPlatform));
				if (entry->skinData) entry->skinData->write(stream);
				stream.writeBool(entry->isTeacher);
				stream.writeBool(entry->isHost);
				stream.writeBool(entry->isSubClient);
				stream.writeUnsignedInt(static_cast<uint32_t>(entry->color.value_or(((static_cast<uint32_t>(255) << 24) | (static_cast<uint32_t>(255) << 16) | (static_cast<uint32_t>(255) << 8) | static_cast<uint32_t>(255)))));
			} else {
				stream.writeUUID(entry->uuid);
			}
		}
		if (type == Type::Add) {
			for (const auto& entry : entries) {
				stream.writeBool(entry->skinData->isVerified);
			}
		}
    }
};

} // namespace bedrock::protocol
