#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class DimensionNameIds : uint16_t {
    Overworld = 0,
    Nether = 1,
    TheEnd = 2,
};
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<DimensionNameIds>(DimensionNameIds value) noexcept {
    switch (value) {
        case DimensionNameIds::Overworld: return "minecraft:overworld";
        case DimensionNameIds::Nether: return "minecraft:nether";
        case DimensionNameIds::TheEnd: return "minecraft:the_end";
        default: return invalid_tag;
    }
}

}
