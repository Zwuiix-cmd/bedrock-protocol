#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CommandParameterTypes : uint8_t {
    Int = 1,
    Val = 3,
    Rval = 4,
    Wildcardint = 5,
    Operator = 6,
    Compareoperator = 7,
    Selection = 8,
    Wildcardselection = 10,
    Pathcommand = 17,
    Fullintegerrange = 23,
    Equipmentslotenum = 47,
    Id = 56,
    Position = 64,
    PositionFloat = 65,
    MessageRoot = 68,
    Rawtext = 70,
    JsonObject = 74,
    BlockStateArray = 84,
    ClockTimemarkerName = 86,
    PoiTag = 87,
    PoiType = 88,
    Codebuilderargs = 90,
};

} // namespace bedrock::protocol
