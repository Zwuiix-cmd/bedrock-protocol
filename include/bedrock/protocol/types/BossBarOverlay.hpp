#pragma once

#include <cstdint>
#include <string_view>

enum class BossBarOverlay : uint8_t {
    Progress = 0,
    Notched6 = 1,
    Notched10 = 2,
    Notched12 = 3,
    Notched20 = 4,
};
