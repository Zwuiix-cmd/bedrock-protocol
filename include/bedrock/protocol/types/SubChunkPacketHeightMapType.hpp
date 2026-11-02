#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class SubChunkPacketHeightMapType : uint8_t {
    NoData = 0,
    Data = 1,
    AllTooHigh = 2,
    AllTooLow = 3,
    AllCopied = 4,
};

} // namespace bedrock::protocol
