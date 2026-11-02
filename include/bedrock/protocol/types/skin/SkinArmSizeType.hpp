#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class SkinArmSizeType : uint16_t {
    Slim = 0,
    Wide = 1,
};
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<SkinArmSizeType>(SkinArmSizeType value) noexcept {
    switch (value) {
        case SkinArmSizeType::Slim: return "slim";
        case SkinArmSizeType::Wide: return "wide";
        default: return invalid_tag;
    }
}

}
