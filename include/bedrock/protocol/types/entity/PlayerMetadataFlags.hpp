#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class PlayerMetadataFlags : uint8_t {
    Sleep = 1,
    Dead = 2,
};
