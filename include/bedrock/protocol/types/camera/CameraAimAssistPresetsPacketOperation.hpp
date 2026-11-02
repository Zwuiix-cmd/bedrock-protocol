#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CameraAimAssistPresetsPacketOperation : uint8_t {
    Set = 0,
    AddToExisting = 1,
};

} // namespace bedrock::protocol
