#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeValueColorType : uint8_t {
    String = 0,
    Array = 1,
};

} // namespace bedrock::protocol
