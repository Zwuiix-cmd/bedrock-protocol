#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class PredictedResult : uint8_t {
    Failure = 0,
    Success = 1,
};
