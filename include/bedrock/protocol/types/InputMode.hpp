#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class InputMode : uint8_t {
    MouseKeyboard = 1,
    Touchscreen = 2,
    GamePad = 3,
};
