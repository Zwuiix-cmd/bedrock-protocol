#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CursorItemDragState : uint8_t {
    Start = 0,
    Stop = 1,
};

} // namespace bedrock::protocol
