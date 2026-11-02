#pragma once

#include <cstdint>
#include <string_view>

enum class CameraAimAssistPresetsPacketOperation : uint8_t {
    Set = 0,
    AddToExisting = 1,
};
