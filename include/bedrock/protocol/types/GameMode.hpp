#pragma once

#include <cstdint>
#include <string_view>

enum class GameMode : uint8_t {
    Survival = 0,
    Creative = 1,
    Adventure = 2,
    SurvivalViewer = 3,
    CreativeViewer = 4,
    Default = 5,
    Spectator = 6,
};
