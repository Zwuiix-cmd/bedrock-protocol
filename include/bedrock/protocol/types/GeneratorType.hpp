#pragma once

#include <cstdint>
#include <string_view>

enum class GeneratorType : uint8_t {
    FiniteOverworld = 0,
    Overworld = 1,
    Flat = 2,
    Nether = 3,
    TheEnd = 4,
};
