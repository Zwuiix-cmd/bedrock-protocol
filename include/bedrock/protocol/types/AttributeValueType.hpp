#pragma once

#include <cstdint>
#include <string_view>

enum class AttributeValueType : uint8_t {
    Bool = 0,
    Float = 1,
    Color = 2,
};
