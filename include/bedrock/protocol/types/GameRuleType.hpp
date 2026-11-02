#pragma once

#include <cstdint>
#include <string_view>

enum class GameRuleType : uint8_t {
    Bool = 1,
    Int = 2,
    Float = 3,
};
