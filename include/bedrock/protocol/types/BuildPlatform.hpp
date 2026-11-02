#pragma once

#include <cstdint>
#include <string_view>

enum class BuildPlatform : int32_t {
    Unknown = -1,
    Android = 1,
    Ios = 2,
    Osx = 3,
    Amazon = 4,
    GearVr = 5,
    Hololens = 6,
    Windows10 = 7,
    Win32 = 8,
    Dedicated = 9,
    Tvos = 10,
    Playstation = 11,
    Nintendo = 12,
    Xbox = 13,
    WindowsPhone = 14,
    Linux = 15,
};
