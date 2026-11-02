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
#include <bedrock/protocol/types/command/CommandOriginData.hpp>
#include <bedrock/protocol/types/command/CommandOutputMessage.hpp>
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
#include <bedrock/protocol/types/command/CommandOriginData.hpp>
#include <bedrock/protocol/types/command/CommandOutputMessage.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class CommandOutputPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::CommandOutput;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr std::string_view TYPE_LAST = "lastoutput";
    static constexpr std::string_view TYPE_SILENT = "silent";
    static constexpr std::string_view TYPE_ALL = "alloutput";
    static constexpr std::string_view TYPE_DATA_SET = "dataset";
    std::shared_ptr<CommandOriginData> originData;
    std::string outputType;
    uint32_t successCount;
    std::vector<std::shared_ptr<CommandOutputMessage>> messages;
    std::optional<std::string> data;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		originData = ([&]() {
		    auto value = std::make_shared<CommandOriginData>();
		    value->read(stream);
		    return value;
		}());
		outputType = stream.readString().value_or("");
		successCount = stream.readUnsignedInt().value_or(0);
		{
		    auto size = stream.readUnsignedVarInt().value_or(0);
		    for (std::size_t i = 0; (i < size); ++i) {
				messages.push_back(([&]() -> std::shared_ptr<CommandOutputMessage> {
					auto message = std::make_shared<CommandOutputMessage>();
					message->messageId = stream.readString().value_or("");
					message->isInternal = stream.readBool().value_or(false);
					{
					    auto size = stream.readUnsignedVarInt().value_or(0);
					    for (std::size_t i = 0; (i < size); ++i) {
							message->parameters.push_back(stream.readString().value_or(""));
					    }
					}
					return message;
				}()));
		    }
		}
		data = stream.readOptional([&]() {
		            return stream.readString().value_or("");
		        });
    }

    void write(BinaryStream& stream) override {
		if (originData) originData->write(stream);
		stream.writeString(outputType);
		stream.writeUnsignedInt(successCount);
		stream.writeUnsignedVarInt(messages.size());
		for (const auto& message : messages) {
			([&]() {
				stream.writeString(message->messageId);
				stream.writeBool(message->isInternal);
				stream.writeUnsignedVarInt(message->parameters.size());
				for (const auto& parameter : message->parameters) {
					stream.writeString(parameter);
				}
			}());
		}
		stream.writeOptional(data, [&](const auto& value) {
		            stream.writeString(value);
		        });
    }
};
