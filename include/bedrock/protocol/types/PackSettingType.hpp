#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class PackSettingType : uint8_t {
    Float = 0,
    Bool = 1,
    String = 2,
};

} // namespace bedrock::protocol
