#pragma once

#include <cstdint>
#include <string_view>

enum class RecipeUnlockingContext : uint8_t {
    None = 0,
    AlwaysUnlocked = 1,
    PlayerInWater = 2,
    PlayerHasManyItems = 3,
};
