#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class DimensionNameIds : uint16_t {
    Overworld = 0,
    Nether = 1,
    TheEnd = 2,
};

inline constexpr std::array<std::string_view, 3> DimensionNameIdsNames = {
#include <bedrock/protocol/types/DimensionNameIdsNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<DimensionNameIds>(DimensionNameIds value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < DimensionNameIdsNames.size() ? customize_t{DimensionNameIdsNames[index]} : invalid_tag;
}

}
