#pragma once

#include <cstdint>
#include <string_view>

enum class SyncWorldClocksType : uint8_t {
    SyncState = 0,
    InitializeRegistry = 1,
    AddTimeMarker = 2,
    RemoveTimeMarker = 3,
};
