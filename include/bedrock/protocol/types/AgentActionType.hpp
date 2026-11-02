#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AgentActionType : uint8_t {
    Attack = 1,
    Collect = 2,
    Destroy = 3,
    DetectRedstone = 4,
    DetectObstacle = 5,
    Drop = 6,
    DropAll = 7,
    Inspect = 8,
    InspectData = 9,
    InspectItemCount = 10,
    InspectItemDetail = 11,
    InspectItemSpace = 12,
    Interact = 13,
    Move = 14,
    PlaceBlock = 15,
    Till = 16,
    TransferItemTo = 17,
    Turn = 18,
};

} // namespace bedrock::protocol
