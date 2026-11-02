#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class UIProfile : uint8_t {
    Classic = 0,
    Pocket = 1,
};
