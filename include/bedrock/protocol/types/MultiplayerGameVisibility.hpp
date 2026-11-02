#pragma once

#include <cstdint>
#include <string_view>

enum class MultiplayerGameVisibility : uint8_t {
    None = 0,
    Invite = 1,
    Friends = 2,
    FriendsOfFriends = 3,
    Public = 4,
};
