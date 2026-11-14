#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class EmoteType : uint8_t {
    Standing = 0,
    Riding = 1,
    Laying = 2,
};

} // namespace bedrock::protocol
