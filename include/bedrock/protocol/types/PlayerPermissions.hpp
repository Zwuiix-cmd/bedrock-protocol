#pragma once

#include <cstdint>
#include <string_view>

enum class PlayerPermissions : uint8_t {
    Custom = 3,
    Operator = 2,
    Member = 1,
    Visitor = 0,
};
