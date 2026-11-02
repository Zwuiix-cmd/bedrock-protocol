#pragma once

#include <array>
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

inline constexpr std::array<std::string_view, 9> MatchmakingStateNames = {
#include <bedrock/protocol/types/MatchmakingStateNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<MatchmakingState>(MatchmakingState value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < MatchmakingStateNames.size() ? customize_t{MatchmakingStateNames[index]} : invalid_tag;
}

}
