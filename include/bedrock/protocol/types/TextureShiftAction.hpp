#pragma once

#include <cstdint>
#include <string_view>

enum class TextureShiftAction : uint8_t {
    Invalid = 0,
    Initialize = 1,
    Start = 2,
    SetEnabled = 3,
    Sync = 4,
};
