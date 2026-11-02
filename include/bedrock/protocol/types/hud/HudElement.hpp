#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class HudElement : uint8_t {
    PaperDoll = 0,
    Armor = 1,
    Tooltips = 2,
    TouchControls = 3,
    Crosshair = 4,
    Hotbar = 5,
    Health = 6,
    Xp = 7,
    Food = 8,
    AirBubbles = 9,
    HorseHealth = 10,
    StatusEffects = 11,
    ItemText = 12,
};
