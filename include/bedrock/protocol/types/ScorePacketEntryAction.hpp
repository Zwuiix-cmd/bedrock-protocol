#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ScorePacketEntryAction : uint16_t {
    Remove = 0,
    ChangePlayer = 1,
    ChangeEntity = 2,
    ChangeFakePlayer = 3,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::ScorePacketEntryAction>(::bedrock::protocol::ScorePacketEntryAction value) noexcept {
    switch (value) {
        case ::bedrock::protocol::ScorePacketEntryAction::Remove: return "remove";
        case ::bedrock::protocol::ScorePacketEntryAction::ChangePlayer: return "changeplayer";
        case ::bedrock::protocol::ScorePacketEntryAction::ChangeEntity: return "changeentity";
        case ::bedrock::protocol::ScorePacketEntryAction::ChangeFakePlayer: return "changefakeplayer";
        default: return invalid_tag;
    }
}

}
