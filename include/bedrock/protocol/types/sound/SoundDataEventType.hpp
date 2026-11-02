#pragma once

#include <cstdint>
#include <string_view>

enum class SoundDataEventType : uint8_t {
    Stop = 0,
    SetVolume = 1,
    SetPitch = 2,
    Fade = 3,
    SeekTo = 4,
    Pause = 5,
    Resume = 6,
};
