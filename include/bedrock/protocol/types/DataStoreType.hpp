#pragma once

#include <cstdint>
#include <string_view>

enum class DataStoreType : uint8_t {
    Update = 0,
    Change = 1,
    Removal = 2,
};
