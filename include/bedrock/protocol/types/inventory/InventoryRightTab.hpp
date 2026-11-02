#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class InventoryRightTab : uint8_t {
    None = 0,
    FullScreen = 1,
    Crafting = 2,
    Armor = 3,
};

} // namespace bedrock::protocol
