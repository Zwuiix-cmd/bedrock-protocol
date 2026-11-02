#pragma once

#include <cstdint>
#include <string_view>

enum class InteractionMode : uint8_t {
    Touch = 0,
    Crosshair = 1,
    Classic = 2,
};
