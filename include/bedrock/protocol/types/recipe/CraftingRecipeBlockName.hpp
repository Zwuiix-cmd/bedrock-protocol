#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CraftingRecipeBlockName : uint16_t {
    CartographyTable = 0,
    CraftingTable = 1,
    Stonecutter = 2,
    SmithingTable = 3,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::CraftingRecipeBlockName>(::bedrock::protocol::CraftingRecipeBlockName value) noexcept {
    switch (value) {
        case ::bedrock::protocol::CraftingRecipeBlockName::CartographyTable: return "cartography_table";
        case ::bedrock::protocol::CraftingRecipeBlockName::CraftingTable: return "crafting_table";
        case ::bedrock::protocol::CraftingRecipeBlockName::Stonecutter: return "stonecutter";
        case ::bedrock::protocol::CraftingRecipeBlockName::SmithingTable: return "smithing_table";
        default: return invalid_tag;
    }
}

}
