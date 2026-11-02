#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class MultiplayerGameVisibility : uint8_t {
    None = 0,
    Invite = 1,
    Friends = 2,
    FriendsOfFriends = 3,
    Public = 4,
};

} // namespace bedrock::protocol
