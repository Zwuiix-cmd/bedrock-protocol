#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class GeneratorType : uint8_t {
    FiniteOverworld = 0,
    Overworld = 1,
    Flat = 2,
    Nether = 3,
    TheEnd = 4,
};

} // namespace bedrock::protocol
