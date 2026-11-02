#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class LevelSoundEvent : uint32_t {
#include <bedrock/protocol/types/LevelSoundEventCases0.inc>
#include <bedrock/protocol/types/LevelSoundEventCases1.inc>
#include <bedrock/protocol/types/LevelSoundEventCases2.inc>
};

} // namespace bedrock::protocol
