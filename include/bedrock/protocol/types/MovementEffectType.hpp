#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class MovementEffectType : int32_t {
    Invalid = -1,
    GlideBoost = 0,
};

} // namespace bedrock::protocol
