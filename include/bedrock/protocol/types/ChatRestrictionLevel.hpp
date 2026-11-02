#pragma once

#include <cstdint>
#include <string_view>

enum class ChatRestrictionLevel : uint8_t {
    None = 0,
    Dropped = 1,
    Disabled = 2,
};
