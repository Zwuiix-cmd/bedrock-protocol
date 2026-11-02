#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class AttributeValueType : uint8_t {
    Bool = 0,
    Float = 1,
    Color = 2,
};
