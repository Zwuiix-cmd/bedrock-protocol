#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CameraSetInstructionEaseType : uint16_t {
    Linear = 0,
    Spring = 1,
    InQuad = 2,
    OutQuad = 3,
    InOutQuad = 4,
    InCubic = 5,
    OutCubic = 6,
    InOutCubic = 7,
    InQuart = 8,
    OutQuart = 9,
    InOutQuart = 10,
    InQuint = 11,
    OutQuint = 12,
    InOutQuint = 13,
    InSine = 14,
    OutSine = 15,
    InOutSine = 16,
    InExpo = 17,
    OutExpo = 18,
    InOutExpo = 19,
    InCirc = 20,
    OutCirc = 21,
    InOutCirc = 22,
    InBounce = 23,
    OutBounce = 24,
    InOutBounce = 25,
    InBack = 26,
    OutBack = 27,
    InOutBack = 28,
    InElastic = 29,
    OutElastic = 30,
    InOutElastic = 31,
    InverseLerp = 32,
};

inline constexpr std::array<std::string_view, 33> CameraSetInstructionEaseTypeNames = {
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseTypeNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CameraSetInstructionEaseType>(CameraSetInstructionEaseType value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < CameraSetInstructionEaseTypeNames.size() ? customize_t{CameraSetInstructionEaseTypeNames[index]} : invalid_tag;
}

}
