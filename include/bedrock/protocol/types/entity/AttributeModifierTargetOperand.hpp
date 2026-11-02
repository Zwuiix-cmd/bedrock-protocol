#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeModifierTargetOperand : uint8_t {
    Min = 0,
    Max = 1,
    Current = 2,
};

} // namespace bedrock::protocol
