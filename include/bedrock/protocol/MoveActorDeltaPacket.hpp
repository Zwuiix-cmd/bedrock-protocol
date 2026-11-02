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
#include <bedrock/protocol/types/DataStoreType.hpp>
#include <bedrock/protocol/types/DataStoreValueType.hpp>
#include <bedrock/protocol/types/BuildPlatform.hpp>
#include <bedrock/protocol/types/DimensionIds.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationEditionOffer.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataFlags.hpp>
#include <bedrock/protocol/types/entity/EntityMetadataProperties.hpp>
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

class MoveActorDeltaPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::MoveActorDelta;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr int64_t FLAG_HAS_X = 1;
    static constexpr int64_t FLAG_HAS_Y = 2;
    static constexpr int64_t FLAG_HAS_Z = 4;
    static constexpr int64_t FLAG_HAS_PITCH = 8;
    static constexpr int64_t FLAG_HAS_YAW = 16;
    static constexpr int64_t FLAG_HAS_HEAD_YAW = 32;
    static constexpr int64_t FLAG_GROUND = 64;
    static constexpr int64_t FLAG_TELEPORT = 128;
    static constexpr int64_t FLAG_FORCE_MOVE_LOCAL_ENTITY = 256;

    uint64_t actorRuntimeId;
    uint16_t flags;
    float xPos = 0;
    float yPos = 0;
    float zPos = 0;
    float pitch = 0.0f;
    float yaw = 0.0f;
    float headYaw = 0.0f;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		actorRuntimeId = stream.readActorRuntimeID().value_or(0);
		flags = stream.readUnsignedShort().value_or(0);
		xPos = ([&]() -> float {
			if (((flags & FLAG_HAS_X) != 0)) {
				return stream.readFloat().value_or(0.0f);
			}
			return 0;
		}());
		yPos = ([&]() -> float {
			if (((flags & FLAG_HAS_Y) != 0)) {
				return stream.readFloat().value_or(0.0f);
			}
			return 0;
		}());
		zPos = ([&]() -> float {
			if (((flags & FLAG_HAS_Z) != 0)) {
				return stream.readFloat().value_or(0.0f);
			}
			return 0;
		}());
		pitch = ([&]() -> float {
			if (((flags & FLAG_HAS_PITCH) != 0)) {
				return stream.readRotationByte().value_or(0);
			}
			return 0;
		}());
		yaw = ([&]() -> float {
			if (((flags & FLAG_HAS_YAW) != 0)) {
				return stream.readRotationByte().value_or(0);
			}
			return 0;
		}());
		headYaw = ([&]() -> float {
			if (((flags & FLAG_HAS_HEAD_YAW) != 0)) {
				return stream.readRotationByte().value_or(0);
			}
			return 0;
		}());
    }

    void write(BinaryStream& stream) override {
		stream.writeActorRuntimeID(actorRuntimeId);
		stream.writeUnsignedShort(flags);
		([&]() {
			if (((flags & FLAG_HAS_X) != 0)) {
				stream.writeFloat(xPos);
			}
		}());
		([&]() {
			if (((flags & FLAG_HAS_Y) != 0)) {
				stream.writeFloat(yPos);
			}
		}());
		([&]() {
			if (((flags & FLAG_HAS_Z) != 0)) {
				stream.writeFloat(zPos);
			}
		}());
		([&]() {
			if (((flags & FLAG_HAS_PITCH) != 0)) {
				stream.writeRotationByte(pitch);
			}
		}());
		([&]() {
			if (((flags & FLAG_HAS_YAW) != 0)) {
				stream.writeRotationByte(yaw);
			}
		}());
		([&]() {
			if (((flags & FLAG_HAS_HEAD_YAW) != 0)) {
				stream.writeRotationByte(headYaw);
			}
		}());
    }
};
