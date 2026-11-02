#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ScorePacketEntryAction : uint16_t {
    Remove = 0,
    ChangePlayer = 1,
    ChangeEntity = 2,
    ChangeFakePlayer = 3,
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ScorePacketEntryAction>(ScorePacketEntryAction value) noexcept {
    switch (value) {
        case ScorePacketEntryAction::Remove: return "remove";
        case ScorePacketEntryAction::ChangePlayer: return "changeplayer";
        case ScorePacketEntryAction::ChangeEntity: return "changeentity";
        case ScorePacketEntryAction::ChangeFakePlayer: return "changefakeplayer";
        default: return invalid_tag;
    }
}

}
