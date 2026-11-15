#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class FurnaceType : uint8_t {
    None = 0,
    Furnace = 1,
    BlastFurnace = 2,
    Smoker = 3,
};

} // namespace bedrock::protocol
