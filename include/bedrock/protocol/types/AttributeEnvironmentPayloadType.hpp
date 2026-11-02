#pragma once

#include <cstdint>
#include <string_view>

enum class AttributeEnvironmentPayloadType : uint8_t {
    Constant = 0,
    Transition = 1,
    NoiseTransition = 2,
};
