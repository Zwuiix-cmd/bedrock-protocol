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
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>
#include <bedrock/protocol/types/recipe/PotionTypeRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionContainerChangeRecipe.hpp>
#include <bedrock/protocol/types/recipe/MaterialReducerRecipe.hpp>
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
#include <bedrock/protocol/types/recipe/MaterialReducerRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionContainerChangeRecipe.hpp>
#include <bedrock/protocol/types/recipe/PotionTypeRecipe.hpp>
#include <bedrock/protocol/types/recipe/RecipeWithTypeId.hpp>
#include <bedrock/protocol/types/TypesGenerated.hpp>

class CraftingDataPacket : public Packet {
public:
    static constexpr PacketIds ID = PacketIds::CraftingData;

    PacketIds getId() override {
        return ID;
    }

public:
    static constexpr int64_t ENTRY_SHAPELESS = 0;
    static constexpr int64_t ENTRY_SHAPED = 1;
    static constexpr int64_t ENTRY_FURNACE = 2;
    static constexpr int64_t ENTRY_FURNACE_DATA = 3;
    static constexpr int64_t ENTRY_MULTI = 4;
    static constexpr int64_t ENTRY_USER_DATA_SHAPELESS = 5;
    static constexpr int64_t ENTRY_SHAPELESS_CHEMISTRY = 6;
    static constexpr int64_t ENTRY_SHAPED_CHEMISTRY = 7;
    static constexpr int64_t ENTRY_SMITHING_TRANSFORM = 8;
    static constexpr int64_t ENTRY_SMITHING_TRIM = 9;

    std::vector<std::shared_ptr<RecipeWithTypeId>> recipesWithTypeIds;
    std::vector<std::shared_ptr<PotionTypeRecipe>> potionTypeRecipes;
    std::vector<std::shared_ptr<PotionContainerChangeRecipe>> potionContainerRecipes;
    std::vector<std::shared_ptr<MaterialReducerRecipe>> materialReducerRecipes;
    bool cleanRecipes = false;

public:
    std::string_view getName() override {
        return magic_enum::enum_name(ID);
    }

    void read(ReadOnlyBinaryStream& stream) override {
		auto recipeCount = stream.readUnsignedVarInt().value_or(0);
		auto previousType = "none";
		for (std::size_t i = 0; (i < recipeCount); ++i) {
			auto recipeType = stream.readVarInt().value_or(0);
			recipesWithTypeIds.push_back(([&]() -> std::shared_ptr<RecipeWithTypeId> {
			    if (recipeType == ENTRY_SHAPELESS) return ShapelessRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_USER_DATA_SHAPELESS) return ShapelessRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_SHAPELESS_CHEMISTRY) return ShapelessRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_SHAPED) return ShapedRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_SHAPED_CHEMISTRY) return ShapedRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_FURNACE) return FurnaceRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_FURNACE_DATA) return FurnaceRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_MULTI) return MultiRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_SMITHING_TRANSFORM) return SmithingTransformRecipe::decode(recipeType, stream);
			    if (recipeType == ENTRY_SMITHING_TRIM) return SmithingTrimRecipe::decode(recipeType, stream);
			    throw std::exception{};
			}()));
			(void)(recipeType);
		}
		{
		    auto count = stream.readUnsignedVarInt().value_or(0);
		    for (std::size_t i = 0; (i < count); ++i) {
				auto inputId = stream.readVarInt().value_or(0);
				auto inputMeta = stream.readVarInt().value_or(0);
				auto ingredientId = stream.readVarInt().value_or(0);
				auto ingredientMeta = stream.readVarInt().value_or(0);
				auto outputId = stream.readVarInt().value_or(0);
				auto outputMeta = stream.readVarInt().value_or(0);
				potionTypeRecipes.push_back(std::make_shared<PotionTypeRecipe>(inputId, inputMeta, ingredientId, ingredientMeta, outputId, outputMeta));
		    }
		}
		{
		    auto count = stream.readUnsignedVarInt().value_or(0);
		    for (std::size_t i = 0; (i < count); ++i) {
				auto input = stream.readVarInt().value_or(0);
				auto ingredient = stream.readVarInt().value_or(0);
				auto output = stream.readVarInt().value_or(0);
				potionContainerRecipes.push_back(std::make_shared<PotionContainerChangeRecipe>(input, ingredient, output));
		    }
		}
		{
		    auto count = stream.readUnsignedVarInt().value_or(0);
		    for (std::size_t i = 0; (i < count); ++i) {
				auto inputIdAndData = stream.readVarInt().value_or(0);
				auto inputId = (inputIdAndData >> 16);
				auto inputMeta = (inputIdAndData & 32767);
				auto outputs = std::vector<std::shared_ptr<MaterialReducerRecipeOutput>>();
				{
				    auto outputCount = stream.readUnsignedVarInt().value_or(0);
				    for (std::size_t j = 0; (j < outputCount); ++j) {
						auto outputItemId = stream.readVarInt().value_or(0);
						auto outputItemCount = stream.readVarInt().value_or(0);
						outputs.push_back(std::make_shared<MaterialReducerRecipeOutput>(outputItemId, outputItemCount));
				    }
				}
				materialReducerRecipes.push_back(std::make_shared<MaterialReducerRecipe>(inputId, inputMeta, outputs));
		    }
		}
		cleanRecipes = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) override {
		stream.writeUnsignedVarInt(recipesWithTypeIds.size());
		for (const auto& d : recipesWithTypeIds) {
			stream.writeVarInt(d->typeId);
			d->write(stream);
		}
		stream.writeUnsignedVarInt(potionTypeRecipes.size());
		for (const auto& recipe : potionTypeRecipes) {
			stream.writeVarInt(recipe->inputItemId);
			stream.writeVarInt(recipe->inputItemMeta);
			stream.writeVarInt(recipe->ingredientItemId);
			stream.writeVarInt(recipe->ingredientItemMeta);
			stream.writeVarInt(recipe->outputItemId);
			stream.writeVarInt(recipe->outputItemMeta);
		}
		stream.writeUnsignedVarInt(potionContainerRecipes.size());
		for (const auto& recipe : potionContainerRecipes) {
			stream.writeVarInt(recipe->inputItemId);
			stream.writeVarInt(recipe->ingredientItemId);
			stream.writeVarInt(recipe->outputItemId);
		}
		stream.writeUnsignedVarInt(materialReducerRecipes.size());
		for (const auto& recipe : materialReducerRecipes) {
			stream.writeVarInt(((recipe->inputItemId << 16) | recipe->inputItemMeta));
			stream.writeUnsignedVarInt(recipe->outputs.size());
			for (const auto& output : recipe->outputs) {
				stream.writeVarInt(output->itemId);
				stream.writeVarInt(output->count);
			}
		}
		stream.writeBool(cleanRecipes);
    }
};
