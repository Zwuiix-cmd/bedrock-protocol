#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class TriggerType : uint8_t {
    Unknown = 0,
    PlayerInput = 1,
    SimulationTick = 2,
};
