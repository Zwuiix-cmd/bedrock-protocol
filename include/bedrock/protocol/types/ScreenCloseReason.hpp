#pragma once

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

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ScreenCloseReason>(ScreenCloseReason value) noexcept {
    switch (value) {
        case ScreenCloseReason::ProgrammaticClose: return "programmaticclose";
        case ScreenCloseReason::ProgrammaticCloseAll: return "programmaticcloseall";
        case ScreenCloseReason::ClientCanceled: return "clientcanceled";
        case ScreenCloseReason::UserBusy: return "userbusy";
        case ScreenCloseReason::InvalidForm: return "invalidform";
        default: return invalid_tag;
    }
}

}
