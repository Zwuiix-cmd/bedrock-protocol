#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

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

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::MatchmakingState>(::bedrock::protocol::MatchmakingState value) noexcept {
    switch (value) {
        case ::bedrock::protocol::MatchmakingState::Idle: return "Idle";
        case ::bedrock::protocol::MatchmakingState::Matchmaking: return "Matchmaking";
        case ::bedrock::protocol::MatchmakingState::MatchFound: return "MatchFound";
        case ::bedrock::protocol::MatchmakingState::Canceled: return "Canceled";
        case ::bedrock::protocol::MatchmakingState::PlayerLeftParty: return "PlayerLeftParty";
        case ::bedrock::protocol::MatchmakingState::PlayerLeftServer: return "PlayerLeftServer";
        case ::bedrock::protocol::MatchmakingState::ServerShutdown: return "ServerShutdown";
        case ::bedrock::protocol::MatchmakingState::TimedOut: return "TimedOut";
        case ::bedrock::protocol::MatchmakingState::RequeueAsParty: return "RequeueAsParty";
        default: return invalid_tag;
    }
}

}
