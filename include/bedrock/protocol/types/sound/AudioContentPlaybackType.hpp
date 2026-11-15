#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AudioContentPlaybackType : uint8_t {
    Music = 0,
    Sound = 1,
};

} // namespace bedrock::protocol
