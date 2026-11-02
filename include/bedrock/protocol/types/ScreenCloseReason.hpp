#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ScreenCloseReason : uint16_t {
    ProgrammaticClose = 0,
    ProgrammaticCloseAll = 1,
    ClientCanceled = 2,
    UserBusy = 3,
    InvalidForm = 4,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::ScreenCloseReason>(::bedrock::protocol::ScreenCloseReason value) noexcept {
    switch (value) {
        case ::bedrock::protocol::ScreenCloseReason::ProgrammaticClose: return "programmaticclose";
        case ::bedrock::protocol::ScreenCloseReason::ProgrammaticCloseAll: return "programmaticcloseall";
        case ::bedrock::protocol::ScreenCloseReason::ClientCanceled: return "clientcanceled";
        case ::bedrock::protocol::ScreenCloseReason::UserBusy: return "userbusy";
        case ::bedrock::protocol::ScreenCloseReason::InvalidForm: return "invalidform";
        default: return invalid_tag;
    }
}

}
