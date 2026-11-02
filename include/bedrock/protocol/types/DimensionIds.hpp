#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class DimensionIds : uint8_t {
    Overworld = 0,
    Nether = 1,
    TheEnd = 2,
};
