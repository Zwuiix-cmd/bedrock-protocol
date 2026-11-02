#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class InventoryLayout : uint8_t {
    None = 0,
    InventoryOnly = 1,
    Default = 2,
    RecipeBookOnly = 3,
};

} // namespace bedrock::protocol
