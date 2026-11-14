#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class OverrideUpdateType : uint16_t {
    ClearOverrides = 0,
    RemoveOverride = 1,
    SetIntOverride = 2,
    SetFloatOverride = 3,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::OverrideUpdateType>(::bedrock::protocol::OverrideUpdateType value) noexcept {
    switch (value) {
        case ::bedrock::protocol::OverrideUpdateType::ClearOverrides: return "clearoverrides";
        case ::bedrock::protocol::OverrideUpdateType::RemoveOverride: return "removeoverride";
        case ::bedrock::protocol::OverrideUpdateType::SetIntOverride: return "setintoverride";
        case ::bedrock::protocol::OverrideUpdateType::SetFloatOverride: return "setfloatoverride";
        default: return invalid_tag;
    }
}

}
