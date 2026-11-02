#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CameraAimAssistTargetMode : uint8_t {
    Angle = 0,
    Distance = 1,
};
