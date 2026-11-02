#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class FurnaceRecipeBlockName : uint16_t {
    BlastFurnace = 0,
    Campfire = 1,
    Furnace = 2,
    Smoker = 3,
    SoulCampfire = 4,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::FurnaceRecipeBlockName>(::bedrock::protocol::FurnaceRecipeBlockName value) noexcept {
    switch (value) {
        case ::bedrock::protocol::FurnaceRecipeBlockName::BlastFurnace: return "blast_furnace";
        case ::bedrock::protocol::FurnaceRecipeBlockName::Campfire: return "campfire";
        case ::bedrock::protocol::FurnaceRecipeBlockName::Furnace: return "furnace";
        case ::bedrock::protocol::FurnaceRecipeBlockName::Smoker: return "smoker";
        case ::bedrock::protocol::FurnaceRecipeBlockName::SoulCampfire: return "soul_campfire";
        default: return invalid_tag;
    }
}

}
