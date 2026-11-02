#pragma once

#include <cstdint>
#include <string_view>

enum class LevelSoundEvent : uint32_t {
#include <bedrock/protocol/types/LevelSoundEventCases0.inc>
#include <bedrock/protocol/types/LevelSoundEventCases1.inc>
#include <bedrock/protocol/types/LevelSoundEventCases2.inc>
};
