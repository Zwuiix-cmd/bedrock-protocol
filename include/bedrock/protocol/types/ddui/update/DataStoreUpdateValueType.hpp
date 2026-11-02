#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class DataStoreUpdateValueType : uint8_t {
    Double = 0,
    Bool = 1,
    String = 2,
};
