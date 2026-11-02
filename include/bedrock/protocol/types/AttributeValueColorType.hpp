#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class AttributeValueColorType : uint8_t {
    String = 0,
    Array = 1,
};
