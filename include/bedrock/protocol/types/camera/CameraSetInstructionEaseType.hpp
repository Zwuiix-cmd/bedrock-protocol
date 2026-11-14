#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

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

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::CameraSetInstructionEaseType>(::bedrock::protocol::CameraSetInstructionEaseType value) noexcept {
    switch (value) {
        case ::bedrock::protocol::CameraSetInstructionEaseType::Linear: return "linear";
        case ::bedrock::protocol::CameraSetInstructionEaseType::Spring: return "spring";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InQuad: return "in_quad";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutQuad: return "out_quad";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutQuad: return "in_out_quad";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InCubic: return "in_cubic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutCubic: return "out_cubic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutCubic: return "in_out_cubic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InQuart: return "in_quart";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutQuart: return "out_quart";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutQuart: return "in_out_quart";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InQuint: return "in_quint";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutQuint: return "out_quint";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutQuint: return "in_out_quint";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InSine: return "in_sine";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutSine: return "out_sine";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutSine: return "in_out_sine";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InExpo: return "in_expo";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutExpo: return "out_expo";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutExpo: return "in_out_expo";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InCirc: return "in_circ";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutCirc: return "out_circ";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutCirc: return "in_out_circ";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InBounce: return "in_bounce";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutBounce: return "out_bounce";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutBounce: return "in_out_bounce";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InBack: return "in_back";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutBack: return "out_back";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutBack: return "in_out_back";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InElastic: return "in_elastic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::OutElastic: return "out_elastic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InOutElastic: return "in_out_elastic";
        case ::bedrock::protocol::CameraSetInstructionEaseType::InverseLerp: return "inverse_lerp";
        default: return invalid_tag;
    }
}

}
