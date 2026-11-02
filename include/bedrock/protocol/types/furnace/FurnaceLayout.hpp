#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class FurnaceLayout : uint8_t {
    None = 0,
    InventoryOnly = 1,
    Default = 2,
};
