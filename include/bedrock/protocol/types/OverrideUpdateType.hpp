#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class OverrideUpdateType : uint16_t {
    ClearOverrides = 0,
    RemoveOverride = 1,
    SetIntOverride = 2,
    SetFloatOverride = 3,
};

inline constexpr std::array<std::string_view, 4> OverrideUpdateTypeNames = {
#include <bedrock/protocol/types/OverrideUpdateTypeNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<OverrideUpdateType>(OverrideUpdateType value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < OverrideUpdateTypeNames.size() ? customize_t{OverrideUpdateTypeNames[index]} : invalid_tag;
}

}
