#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class OverrideUpdateType : uint16_t {
    ClearOverrides = 0,
    RemoveOverride = 1,
    SetIntOverride = 2,
    SetFloatOverride = 3,
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<OverrideUpdateType>(OverrideUpdateType value) noexcept {
    switch (value) {
        case OverrideUpdateType::ClearOverrides: return "clearoverrides";
        case OverrideUpdateType::RemoveOverride: return "removeoverride";
        case OverrideUpdateType::SetIntOverride: return "setintoverride";
        case OverrideUpdateType::SetFloatOverride: return "setfloatoverride";
        default: return invalid_tag;
    }
}

}
