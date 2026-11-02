#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ScorePacketEntryAction : uint16_t {
    Remove = 0,
    ChangePlayer = 1,
    ChangeEntity = 2,
    ChangeFakePlayer = 3,
};

inline constexpr std::array<std::string_view, 4> ScorePacketEntryActionNames = {
#include <bedrock/protocol/types/ScorePacketEntryActionNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ScorePacketEntryAction>(ScorePacketEntryAction value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < ScorePacketEntryActionNames.size() ? customize_t{ScorePacketEntryActionNames[index]} : invalid_tag;
}

}
