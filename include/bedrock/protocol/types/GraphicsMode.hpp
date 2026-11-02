#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class GraphicsMode : uint8_t {
    Simple = 0,
    Fancy = 1,
    Advanced = 2,
    RayTraced = 3,
};

} // namespace bedrock::protocol
