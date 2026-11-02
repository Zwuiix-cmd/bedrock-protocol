#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class TextureShiftAction : uint8_t {
    Invalid = 0,
    Initialize = 1,
    Start = 2,
    SetEnabled = 3,
    Sync = 4,
};
