#pragma once

#include <cstdint>
#include <string_view>

enum class FurnaceType : uint8_t {
    None = 0,
    Furnace = 1,
    BlastFurnace = 2,
    Smoker = 3,
};
