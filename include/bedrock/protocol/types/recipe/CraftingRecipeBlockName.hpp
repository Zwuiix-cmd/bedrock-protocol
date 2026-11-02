#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CraftingRecipeBlockName : uint16_t {
    CartographyTable = 0,
    CraftingTable = 1,
    Stonecutter = 2,
    SmithingTable = 3,
};

inline constexpr std::array<std::string_view, 4> CraftingRecipeBlockNameNames = {
#include <bedrock/protocol/types/recipe/CraftingRecipeBlockNameNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CraftingRecipeBlockName>(CraftingRecipeBlockName value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < CraftingRecipeBlockNameNames.size() ? customize_t{CraftingRecipeBlockNameNames[index]} : invalid_tag;
}

}
