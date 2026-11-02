#pragma once

#include <array>
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

inline constexpr std::array<std::string_view, 5> FurnaceRecipeBlockNameNames = {
#include <bedrock/protocol/types/recipe/FurnaceRecipeBlockNameNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<FurnaceRecipeBlockName>(FurnaceRecipeBlockName value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < FurnaceRecipeBlockNameNames.size() ? customize_t{FurnaceRecipeBlockNameNames[index]} : invalid_tag;
}

}
