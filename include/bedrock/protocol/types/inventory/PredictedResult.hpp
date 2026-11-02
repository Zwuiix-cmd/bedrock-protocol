#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class PredictedResult : uint8_t {
    Failure = 0,
    Success = 1,
};

} // namespace bedrock::protocol
