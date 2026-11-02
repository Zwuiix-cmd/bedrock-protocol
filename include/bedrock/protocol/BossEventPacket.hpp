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
#include <bedrock/protocol/types/TypesGenerated.hpp>

class BossEventPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::BossEvent;

    PacketIds getId() override {
        return ID;
    }

public:
    enum class Type : uint8_t {
        Show = 0,
        RegisterPlayer = 1,
        Hide = 2,
        UnregisterPlayer = 3,
        HealthPercent = 4,
        Title = 5,
        Properties = 6,
        Texture = 7,
        Query = 8,
    };

    int64_t bossActorUniqueId;
    Type eventType;
    int64_t playerActorUniqueId;
    float healthPercent;
    std::string title;
    std::string filteredTitle;
    bool darkenScreen;
    uint32_t color;
    uint32_t overlay;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		std::remove_cvref_t<decltype(stream.readUnsignedShort().value_or(0))> raw{};
		bossActorUniqueId = stream.readActorUniqueID().value_or(0);
		eventType = magic_enum::enum_cast<Type>(
			stream.readUnsignedVarInt().value_or(0)
		).value_or(Type::Show);
		switch (eventType) {
			case Type::RegisterPlayer:
			case Type::UnregisterPlayer:
			case Type::Query: {
				playerActorUniqueId = stream.readActorUniqueID().value_or(0);
				break;
			}
			case Type::Show: {
				title = stream.readString().value_or("");
				filteredTitle = stream.readString().value_or("");
				healthPercent = stream.readFloat().value_or(0.0f);
			}
			case Type::Properties: {
				darkenScreen = stream.readUnsignedShort().value_or(0) != 0;
			}
			case Type::Texture: {
				color = stream.readUnsignedVarInt().value_or(0);
				overlay = stream.readUnsignedVarInt().value_or(0);
				break;
			}
			case Type::HealthPercent: {
				healthPercent = stream.readFloat().value_or(0.0f);
				break;
			}
			case Type::Title: {
				title = stream.readString().value_or("");
				filteredTitle = stream.readString().value_or("");
				break;
			}
			default: {
				break;
			}
		}
    }

    void write(BinaryStream& stream) override {
		stream.writeActorUniqueID(bossActorUniqueId);
		stream.writeUnsignedVarInt(magic_enum::enum_integer(eventType));
		switch (eventType) {
			case Type::RegisterPlayer:
			case Type::UnregisterPlayer:
			case Type::Query: {
				stream.writeActorUniqueID(playerActorUniqueId);
				break;
			}
			case Type::Show: {
				stream.writeString(title);
				stream.writeString(filteredTitle);
				stream.writeFloat(healthPercent);
			}
			case Type::Properties: {
				stream.writeUnsignedShort((darkenScreen ? 1 : 0));
			}
			case Type::Texture: {
				stream.writeUnsignedVarInt(color);
				stream.writeUnsignedVarInt(overlay);
				break;
			}
			case Type::HealthPercent: {
				stream.writeFloat(healthPercent);
				break;
			}
			case Type::Title: {
				stream.writeString(title);
				stream.writeString(filteredTitle);
				break;
			}
			default: {
				break;
			}
		}
    }
};
