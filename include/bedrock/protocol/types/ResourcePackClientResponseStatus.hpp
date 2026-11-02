#pragma once

#include <cstdint>
#include <string_view>

enum class ResourcePackClientResponseStatus : uint16_t {
    Refused = 0,
    SendPacks = 1,
    HaveAllPacks = 2,
    Completed = 3,
};
