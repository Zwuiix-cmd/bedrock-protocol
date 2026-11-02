#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class SkinArmSizeType : uint16_t {
    Slim = 0,
    Wide = 1,
};

inline constexpr std::array<std::string_view, 2> SkinArmSizeTypeNames = {
#include <bedrock/protocol/types/skin/SkinArmSizeTypeNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<SkinArmSizeType>(SkinArmSizeType value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < SkinArmSizeTypeNames.size() ? customize_t{SkinArmSizeTypeNames[index]} : invalid_tag;
}

}
