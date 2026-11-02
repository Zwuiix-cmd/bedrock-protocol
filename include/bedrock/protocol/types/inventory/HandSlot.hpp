#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class HandSlot : uint8_t {
    Mainhand = 0,
    Offhand = 1,
};
