#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ControlScheme : uint8_t {
    LockedPlayerRelativeStrafe = 0,
    CameraRelative = 1,
    CameraRelativeStrafe = 2,
    PlayerRelative = 3,
    PlayerRelativeStrafe = 4,
};
