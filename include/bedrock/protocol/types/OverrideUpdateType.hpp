#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class OverrideUpdateType : uint8_t {
    ClearOverrides = 0,
    RemoveOverride = 1,
    SetIntOverride = 2,
    SetFloatOverride = 3,
};

} // namespace bedrock::protocol
