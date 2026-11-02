#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CompressionAlgorithm : uint8_t {
    Zlib = 0,
    Snappy = 1,
    None = 255,
};

} // namespace bedrock::protocol
