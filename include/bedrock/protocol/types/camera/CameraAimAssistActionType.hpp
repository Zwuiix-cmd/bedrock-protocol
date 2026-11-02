#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CameraAimAssistActionType : uint8_t {
    Set = 0,
    Clear = 1,
};
