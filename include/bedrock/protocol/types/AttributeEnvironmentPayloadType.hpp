#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeEnvironmentPayloadType : uint8_t {
    Constant = 0,
    Transition = 1,
    NoiseTransition = 2,
};

} // namespace bedrock::protocol
