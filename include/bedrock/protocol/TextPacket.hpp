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

class TextPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::Text;

    PacketIds getId() override {
        return ID;
    }

public:
    enum class Category : uint8_t {
        MessageOnly = 0,
        AuthoredMessage = 1,
        MessageWithParameters = 2,
    };

    enum class Type : uint8_t {
        Raw = 0,
        Chat = 1,
        Translation = 2,
        Popup = 3,
        JukeboxPopup = 4,
        Tip = 5,
        System = 6,
        Whisper = 7,
        Announcement = 8,
        JsonWhisper = 9,
        Json = 10,
        JsonAnnouncement = 11,
    };

    Type type;
    bool needsTranslation = false;
    std::string sourceName;
    std::string message;
    std::vector<std::string> parameters;
    std::string xboxUserId;
    std::string platformChatId;
    std::optional<std::string> filteredMessage;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		needsTranslation = stream.readBool().value_or(false);
		auto category = stream.readUnsignedByte().value_or(0);
		type = magic_enum::enum_cast<Type>(
			stream.readUnsignedByte().value_or(0)
		).value_or(Type::Raw);
		switch (type) {
			case Type::Chat:
			case Type::Whisper:
			case Type::Announcement: {
				if (category != magic_enum::enum_integer(Category::AuthoredMessage)) {
					throw std::exception{};
				}
				sourceName = stream.readString().value_or("");
				message = stream.readString().value_or("");
				break;
			}
			case Type::Raw:
			case Type::Tip:
			case Type::System:
			case Type::JsonWhisper:
			case Type::Json:
			case Type::JsonAnnouncement: {
				if (category != magic_enum::enum_integer(Category::MessageOnly)) {
					throw std::exception{};
				}
				message = stream.readString().value_or("");
				break;
			}
			case Type::Translation:
			case Type::Popup:
			case Type::JukeboxPopup: {
				if (category != magic_enum::enum_integer(Category::MessageWithParameters)) {
					throw std::exception{};
				}
				message = stream.readString().value_or("");
				auto count = stream.readUnsignedVarInt().value_or(0);
				    const bool countAvailable = stream.canReadCollection(count);
				for (std::size_t i = 0; (i < count) && !stream.hasOverflowed() && countAvailable && !stream.hasOverflowed(); ++i) {
					parameters.push_back(stream.readString().value_or(""));
				}
				break;
			}
		}
		xboxUserId = stream.readString().value_or("");
		platformChatId = stream.readString().value_or("");
		filteredMessage = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
    }

    void write(BinaryStream& stream) override {
		stream.writeBool(needsTranslation);
		auto category = Category{};
		switch (type) {
			case Type::Raw:
			case Type::Tip:
			case Type::System:
			case Type::JsonWhisper:
			case Type::JsonAnnouncement:
			case Type::Json:
			{
				category = Category::MessageOnly;
				break;
			}
			case Type::Chat:
			case Type::Whisper:
			case Type::Announcement:
			{
				category = Category::AuthoredMessage;
				break;
			}
			case Type::Translation:
			case Type::Popup:
			case Type::JukeboxPopup:
			{
				category = Category::MessageWithParameters;
				break;
			}
			default:
			{
				throw std::exception{};
			}
		}
		stream.writeUnsignedByte(magic_enum::enum_integer(category));
		stream.writeUnsignedByte(magic_enum::enum_integer(type));
		switch (type) {
			case Type::Chat:
			case Type::Whisper:
			case Type::Announcement: {
				stream.writeString(sourceName);
			}
			case Type::Raw:
			case Type::Tip:
			case Type::System:
			case Type::JsonWhisper:
			case Type::Json:
			case Type::JsonAnnouncement: {
				stream.writeString(message);
				break;
			}
			case Type::Translation:
			case Type::Popup:
			case Type::JukeboxPopup: {
				stream.writeString(message);
				stream.writeUnsignedVarInt(parameters.size());
				for (const auto& p : parameters) {
					stream.writeString(p);
				}
				break;
			}
		}
		stream.writeString(xboxUserId);
		stream.writeString(platformChatId);
		stream.writeOptional(filteredMessage, [&](const auto& value) {
			stream.writeString(value);
		});
    }
};
