#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeModifierOperation : uint8_t {
    Add = 0,
    MultiplyBase = 1,
    MultiplyTotal = 2,
    Cap = 3,
};

} // namespace bedrock::protocol
