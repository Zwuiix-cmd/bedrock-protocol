#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ItemDescriptorType : uint8_t {
    IntIdMeta = 1,
    Molang = 2,
    Tag = 3,
    StringIdMeta = 4,
    ComplexAlias = 5,
};
