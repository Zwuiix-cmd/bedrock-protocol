#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class LevelSoundEvent : uint16_t {
#include <bedrock/protocol/types/LevelSoundEventCases0.inc>
#include <bedrock/protocol/types/LevelSoundEventCases1.inc>
#include <bedrock/protocol/types/LevelSoundEventCases2.inc>
};

inline constexpr std::array<std::string_view, 575> LevelSoundEventNames = {
#include <bedrock/protocol/types/LevelSoundEventNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<LevelSoundEvent>(LevelSoundEvent value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < LevelSoundEventNames.size() ? customize_t{LevelSoundEventNames[index]} : invalid_tag;
}

}
