#pragma once

#include <cstdint>
#include <string_view>

enum class PlayerLocationType : uint8_t {
    PlayerLocationCoordinates = 0,
    PlayerLocationHide = 1,
};
