#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class HudVisibility : uint8_t {
    Hide = 0,
    Reset = 1,
};

} // namespace bedrock::protocol
