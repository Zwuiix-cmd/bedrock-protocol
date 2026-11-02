#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CraftingRecipeBlockName : uint16_t {
    CartographyTable = 0,
    CraftingTable = 1,
    Stonecutter = 2,
    SmithingTable = 3,
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CraftingRecipeBlockName>(CraftingRecipeBlockName value) noexcept {
    switch (value) {
        case CraftingRecipeBlockName::CartographyTable: return "cartography_table";
        case CraftingRecipeBlockName::CraftingTable: return "crafting_table";
        case CraftingRecipeBlockName::Stonecutter: return "stonecutter";
        case CraftingRecipeBlockName::SmithingTable: return "smithing_table";
        default: return invalid_tag;
    }
}

}
