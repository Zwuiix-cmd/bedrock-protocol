#pragma once

#include <cstdint>
#include <string_view>

enum class PredictedResult : uint8_t {
    Failure = 0,
    Success = 1,
};
