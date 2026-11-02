#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class PlayMode : uint8_t {
    Normal = 0,
    Teaser = 1,
    Screen = 2,
    ExitLevel = 7,
};

} // namespace bedrock::protocol
