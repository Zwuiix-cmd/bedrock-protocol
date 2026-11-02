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
#include <bedrock/protocol/types/recipe/ShapedRecipe.hpp>
#include <bedrock/protocol/types/recipe/ShapelessRecipe.hpp>
#include <bedrock/protocol/types/recipe/MultiRecipe.hpp>
#include <bedrock/protocol/types/recipe/SmithingTransformRecipe.hpp>
#include <bedrock/protocol/types/recipe/SmithingTrimRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionTypeRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionContainerChangeRecipe.hpp>
#include <bedrock/protocol/types/recipe/MaterialReducerRecipe.hpp>
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
#include <bedrock/protocol/types/recipe/MaterialReducerRecipe.hpp>
#include <bedrock/protocol/types/recipe/MultiRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionContainerChangeRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionTypeRecipe.hpp>
#include <bedrock/protocol/types/recipe/ShapedRecipe.hpp>
#include <bedrock/protocol/types/recipe/ShapelessRecipe.hpp>
#include <bedrock/protocol/types/recipe/SmithingTransformRecipe.hpp>
#include <bedrock/protocol/types/recipe/SmithingTrimRecipe.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class CraftingDataPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::CraftingData;

    PacketIds getId() override {
        return ID;
    }

public:
    std::vector<std::shared_ptr<ShapedRecipe>> shapedRecipes;
    std::vector<std::shared_ptr<ShapelessRecipe>> shapelessRecipes;
    std::vector<std::shared_ptr<MultiRecipe>> multiRecipes;
    std::vector<std::shared_ptr<ShapelessRecipe>> userDataShapelessRecipes;
    std::vector<std::shared_ptr<ShapelessRecipe>> shapelessChemistryRecipes;
    std::vector<std::shared_ptr<ShapedRecipe>> shapedChemistryRecipes;
    std::vector<std::shared_ptr<SmithingTransformRecipe>> smithingTransformRecipes;
    std::vector<std::shared_ptr<SmithingTrimRecipe>> smithingTrimRecipes;
    std::vector<std::shared_ptr<PotionTypeRecipe>> potionTypeRecipes;
    std::vector<std::shared_ptr<PotionContainerChangeRecipe>> potionContainerRecipes;
    std::vector<std::shared_ptr<MaterialReducerRecipe>> materialReducerRecipes;
    bool cleanRecipes = false;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		shapedRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<ShapedRecipe>();
		    value->read(stream);
		    return value;
		}()));
		shapelessRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<ShapelessRecipe>();
		    value->read(stream);
		    return value;
		}()));
		multiRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<MultiRecipe>();
		    value->read(stream);
		    return value;
		}()));
		userDataShapelessRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<ShapelessRecipe>();
		    value->read(stream);
		    return value;
		}()));
		shapelessChemistryRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<ShapelessRecipe>();
		    value->read(stream);
		    return value;
		}()));
		shapedChemistryRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<ShapedRecipe>();
		    value->read(stream);
		    return value;
		}()));
		smithingTransformRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<SmithingTransformRecipe>();
		    value->read(stream);
		    return value;
		}()));
		smithingTrimRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<SmithingTrimRecipe>();
		    value->read(stream);
		    return value;
		}()));
		potionTypeRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<PotionTypeRecipe>();
		    value->read(stream);
		    return value;
		}()));
		potionContainerRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<PotionContainerChangeRecipe>();
		    value->read(stream);
		    return value;
		}()));
		materialReducerRecipes = stream.readList(([&]() {
		    auto value = std::make_shared<MaterialReducerRecipe>();
		    value->read(stream);
		    return value;
		}()));
		cleanRecipes = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) override {
		stream.writeList(shapedRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(shapelessRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(multiRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(userDataShapelessRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(shapelessChemistryRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(shapedChemistryRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(smithingTransformRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(smithingTrimRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(potionTypeRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(potionContainerRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeList(materialReducerRecipes, [&](const auto& recipe) {
		            return recipe->write(stream);
		        });
		stream.writeBool(cleanRecipes);
    }
};
