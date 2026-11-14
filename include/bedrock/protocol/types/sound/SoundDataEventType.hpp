#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class SoundDataEventType : uint8_t {
    Stop = 0,
    SetVolume = 1,
    SetPitch = 2,
    Fade = 3,
    SeekTo = 4,
    Pause = 5,
    Resume = 6,
};

} // namespace bedrock::protocol
