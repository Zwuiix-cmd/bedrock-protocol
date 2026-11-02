#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ItemDescriptorType : uint16_t {
    Empty = 0,
    StringIdMeta = 1,
    Molang = 2,
    Tag = 3,
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ItemDescriptorType>(ItemDescriptorType value) noexcept {
    switch (value) {
        case ItemDescriptorType::Empty: return "empty";
        case ItemDescriptorType::StringIdMeta: return "name";
        case ItemDescriptorType::Molang: return "molang";
        case ItemDescriptorType::Tag: return "item_tag";
        default: return invalid_tag;
    }
}

}
