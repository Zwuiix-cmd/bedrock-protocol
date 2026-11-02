#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class InventoryLeftTab : uint8_t {
    None = 0,
    Construction = 1,
    Equipment = 2,
    Items = 3,
    Nature = 4,
    Search = 5,
    Survival = 6,
};
