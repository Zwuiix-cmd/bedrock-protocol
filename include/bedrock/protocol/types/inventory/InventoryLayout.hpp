#pragma once

#include <cstdint>
#include <string_view>

enum class InventoryLayout : uint8_t {
    None = 0,
    InventoryOnly = 1,
    Default = 2,
    RecipeBookOnly = 3,
};
