#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class SkinArmSizeType : uint16_t {
    Slim = 0,
    Wide = 1,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::SkinArmSizeType>(::bedrock::protocol::SkinArmSizeType value) noexcept {
    switch (value) {
        case ::bedrock::protocol::SkinArmSizeType::Slim: return "slim";
        case ::bedrock::protocol::SkinArmSizeType::Wide: return "wide";
        default: return invalid_tag;
    }
}

}
