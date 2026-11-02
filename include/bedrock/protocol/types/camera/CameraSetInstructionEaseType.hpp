#pragma once

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

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CameraSetInstructionEaseType>(CameraSetInstructionEaseType value) noexcept {
    switch (value) {
        case CameraSetInstructionEaseType::Linear: return "linear";
        case CameraSetInstructionEaseType::Spring: return "spring";
        case CameraSetInstructionEaseType::InQuad: return "in_quad";
        case CameraSetInstructionEaseType::OutQuad: return "out_quad";
        case CameraSetInstructionEaseType::InOutQuad: return "in_out_quad";
        case CameraSetInstructionEaseType::InCubic: return "in_cubic";
        case CameraSetInstructionEaseType::OutCubic: return "out_cubic";
        case CameraSetInstructionEaseType::InOutCubic: return "in_out_cubic";
        case CameraSetInstructionEaseType::InQuart: return "in_quart";
        case CameraSetInstructionEaseType::OutQuart: return "out_quart";
        case CameraSetInstructionEaseType::InOutQuart: return "in_out_quart";
        case CameraSetInstructionEaseType::InQuint: return "in_quint";
        case CameraSetInstructionEaseType::OutQuint: return "out_quint";
        case CameraSetInstructionEaseType::InOutQuint: return "in_out_quint";
        case CameraSetInstructionEaseType::InSine: return "in_sine";
        case CameraSetInstructionEaseType::OutSine: return "out_sine";
        case CameraSetInstructionEaseType::InOutSine: return "in_out_sine";
        case CameraSetInstructionEaseType::InExpo: return "in_expo";
        case CameraSetInstructionEaseType::OutExpo: return "out_expo";
        case CameraSetInstructionEaseType::InOutExpo: return "in_out_expo";
        case CameraSetInstructionEaseType::InCirc: return "in_circ";
        case CameraSetInstructionEaseType::OutCirc: return "out_circ";
        case CameraSetInstructionEaseType::InOutCirc: return "in_out_circ";
        case CameraSetInstructionEaseType::InBounce: return "in_bounce";
        case CameraSetInstructionEaseType::OutBounce: return "out_bounce";
        case CameraSetInstructionEaseType::InOutBounce: return "in_out_bounce";
        case CameraSetInstructionEaseType::InBack: return "in_back";
        case CameraSetInstructionEaseType::OutBack: return "out_back";
        case CameraSetInstructionEaseType::InOutBack: return "in_out_back";
        case CameraSetInstructionEaseType::InElastic: return "in_elastic";
        case CameraSetInstructionEaseType::OutElastic: return "out_elastic";
        case CameraSetInstructionEaseType::InOutElastic: return "in_out_elastic";
        case CameraSetInstructionEaseType::InverseLerp: return "inverse_lerp";
        default: return invalid_tag;
    }
}

}
