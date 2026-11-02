#pragma once

#include <cstdint>
#include <string_view>

enum class GraphicsMode : uint8_t {
    Simple = 0,
    Fancy = 1,
    Advanced = 2,
    RayTraced = 3,
};
