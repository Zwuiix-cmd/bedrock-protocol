#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class FurnaceRecipeBlockName : uint16_t {
    BlastFurnace = 0,
    Campfire = 1,
    Furnace = 2,
    Smoker = 3,
    SoulCampfire = 4,
};
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<FurnaceRecipeBlockName>(FurnaceRecipeBlockName value) noexcept {
    switch (value) {
        case FurnaceRecipeBlockName::BlastFurnace: return "blast_furnace";
        case FurnaceRecipeBlockName::Campfire: return "campfire";
        case FurnaceRecipeBlockName::Furnace: return "furnace";
        case FurnaceRecipeBlockName::Smoker: return "smoker";
        case FurnaceRecipeBlockName::SoulCampfire: return "soul_campfire";
        default: return invalid_tag;
    }
}

}
