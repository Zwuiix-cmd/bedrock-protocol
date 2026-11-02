#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ArmorSlot : uint8_t {
    Head = 0,
    Torso = 1,
    Legs = 2,
    Feet = 3,
    Body = 4,
};

} // namespace bedrock::protocol
