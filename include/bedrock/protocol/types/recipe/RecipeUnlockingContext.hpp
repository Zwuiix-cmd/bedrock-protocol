#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class RecipeUnlockingContext : uint8_t {
    None = 0,
    AlwaysUnlocked = 1,
    PlayerInWater = 2,
    PlayerHasManyItems = 3,
};

} // namespace bedrock::protocol
