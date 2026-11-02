#pragma once

#include <cstdint>
#include <string_view>

enum class FurnaceLeftTab : uint8_t {
    None = 0,
    RecipeFood = 1,
    RecipeItems = 2,
    RecipeBlocks = 3,
    RecipeSearch = 4,
    Inventory = 5,
};
