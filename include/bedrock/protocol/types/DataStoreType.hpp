#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class DataStoreType : uint8_t {
    Update = 0,
    Change = 1,
    Removal = 2,
};

} // namespace bedrock::protocol
