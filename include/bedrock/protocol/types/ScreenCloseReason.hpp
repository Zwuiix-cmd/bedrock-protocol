#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ScreenCloseReason : uint16_t {
    ProgrammaticClose = 0,
    ProgrammaticCloseAll = 1,
    ClientCanceled = 2,
    UserBusy = 3,
    InvalidForm = 4,
};

inline constexpr std::array<std::string_view, 5> ScreenCloseReasonNames = {
#include <bedrock/protocol/types/ScreenCloseReasonNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ScreenCloseReason>(ScreenCloseReason value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < ScreenCloseReasonNames.size() ? customize_t{ScreenCloseReasonNames[index]} : invalid_tag;
}

}
