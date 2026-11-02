#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class NoiseAlignmentType : uint8_t {
    MinLocalTransitionEnd = 0,
};
