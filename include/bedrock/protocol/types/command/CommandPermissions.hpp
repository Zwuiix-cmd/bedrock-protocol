#pragma once

#include <cstdint>
#include <string_view>

enum class CommandPermissions : uint16_t {
    Normal = 0,
    Operator = 1,
    Automation = 2,
    Host = 3,
    Owner = 4,
    Internal = 5,
};
