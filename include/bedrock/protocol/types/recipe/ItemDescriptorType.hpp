#pragma once

#include <cstdint>
#include <string_view>

enum class ItemDescriptorType : uint16_t {
    Empty = 0,
    StringIdMeta = 1,
    Molang = 2,
    Tag = 3,
};
