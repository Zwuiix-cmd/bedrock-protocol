#pragma once

#include <cstdint>
#include <string_view>

enum class ItemStackRequestActionType : uint8_t {
    Take = 0,
    Place = 1,
    Swap = 2,
    Drop = 3,
    Destroy = 4,
    CraftingConsumeInput = 5,
    CraftingCreateSpecificResult = 6,
    LabTableCombine = 9,
    BeaconPayment = 10,
    MineBlock = 11,
    CraftingRecipe = 12,
    CraftingRecipeAuto = 13,
    CreativeCreate = 14,
    CraftingRecipeOptional = 15,
    CraftingGrindstone = 16,
    CraftingLoom = 17,
    CraftingNonImplementedDeprecatedAskTyLaing = 18,
    CraftingResultsDeprecatedAskTyLaing = 19,
};
