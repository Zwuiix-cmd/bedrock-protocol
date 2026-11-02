#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class GameRuleType : uint8_t {
    Null = 0,
    Bool = 1,
    Int = 2,
    Float = 3,
};
