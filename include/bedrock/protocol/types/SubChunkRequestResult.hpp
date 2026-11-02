#pragma once

#include <cstdint>
#include <string_view>

enum class SubChunkRequestResult : uint8_t {
    Success = 1,
    NoSuchChunk = 2,
    WrongDimension = 3,
    NullPlayer = 4,
    YIndexOutOfBounds = 5,
    SuccessAllAir = 6,
};
