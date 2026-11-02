#pragma once

#include <cstdint>
#include <string_view>

enum class ScorePacketEntryAction : uint16_t {
    Remove = 0,
    ChangePlayer = 1,
    ChangeEntity = 2,
    ChangeFakePlayer = 3,
};
