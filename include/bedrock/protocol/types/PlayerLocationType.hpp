#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class PlayerLocationType : uint8_t {
    PlayerLocationCoordinates = 0,
    PlayerLocationHide = 1,
};

} // namespace bedrock::protocol
