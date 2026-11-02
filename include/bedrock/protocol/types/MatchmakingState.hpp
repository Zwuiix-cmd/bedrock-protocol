#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class MatchmakingState : uint16_t {
    Idle = 0,
    Matchmaking = 1,
    MatchFound = 2,
    Canceled = 3,
    PlayerLeftParty = 4,
    PlayerLeftServer = 5,
    ServerShutdown = 6,
    TimedOut = 7,
    RequeueAsParty = 8,
};
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<MatchmakingState>(MatchmakingState value) noexcept {
    switch (value) {
        case MatchmakingState::Idle: return "Idle";
        case MatchmakingState::Matchmaking: return "Matchmaking";
        case MatchmakingState::MatchFound: return "MatchFound";
        case MatchmakingState::Canceled: return "Canceled";
        case MatchmakingState::PlayerLeftParty: return "PlayerLeftParty";
        case MatchmakingState::PlayerLeftServer: return "PlayerLeftServer";
        case MatchmakingState::ServerShutdown: return "ServerShutdown";
        case MatchmakingState::TimedOut: return "TimedOut";
        case MatchmakingState::RequeueAsParty: return "RequeueAsParty";
        default: return invalid_tag;
    }
}

}
