#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ItemDescriptorType : uint16_t {
    Empty = 0,
    StringIdMeta = 1,
    Molang = 2,
    Tag = 3,
};

inline constexpr std::array<std::string_view, 4> ItemDescriptorTypeNames = {
#include <bedrock/protocol/types/recipe/ItemDescriptorTypeNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ItemDescriptorType>(ItemDescriptorType value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < ItemDescriptorTypeNames.size() ? customize_t{ItemDescriptorTypeNames[index]} : invalid_tag;
}

}
