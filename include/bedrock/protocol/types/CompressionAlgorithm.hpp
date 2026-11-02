#pragma once

#include <cstdint>
#include <string_view>

enum class CompressionAlgorithm : uint8_t {
    Zlib = 0,
    Snappy = 1,
    None = 255,
};
