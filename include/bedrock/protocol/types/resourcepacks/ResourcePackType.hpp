#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ResourcePackType : uint8_t {
    Invalid = 0,
    Addon = 1,
    Cached = 2,
    CopyProtected = 3,
    Behaviors = 4,
    PersonaPiece = 5,
    Resources = 6,
    Skins = 7,
    WorldTemplate = 8,
};

} // namespace bedrock::protocol
