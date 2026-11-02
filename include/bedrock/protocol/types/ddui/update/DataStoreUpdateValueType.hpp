#pragma once

#include <cstdint>
#include <string_view>

enum class DataStoreUpdateValueType : uint8_t {
    Double = 0,
    Bool = 1,
    String = 2,
};
