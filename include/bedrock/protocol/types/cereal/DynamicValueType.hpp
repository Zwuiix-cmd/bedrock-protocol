#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class DynamicValueType : uint8_t {
    Null = 0,
    Bool = 1,
    Long = 2,
    Double = 3,
    String = 4,
    List = 5,
    Map = 6,
};
