#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class InputMode : uint8_t {
    Undefined = 0,
    MouseKeyboard = 1,
    Touchscreen = 2,
    GamePad = 3,
    MotionController = 4,
};

} // namespace bedrock::protocol
