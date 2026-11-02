#pragma once

#include <cstdint>
#include <string_view>

enum class OverrideUpdateType : uint8_t {
    ClearOverrides = 0,
    RemoveOverride = 1,
    SetIntOverride = 2,
    SetFloatOverride = 3,
};
