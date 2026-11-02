#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class SyncWorldClocksType : uint8_t {
    SyncState = 0,
    InitializeRegistry = 1,
    AddTimeMarker = 2,
    RemoveTimeMarker = 3,
};

} // namespace bedrock::protocol
