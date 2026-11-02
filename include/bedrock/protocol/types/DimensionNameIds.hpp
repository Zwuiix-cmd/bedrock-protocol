#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class DimensionNameIds : uint16_t {
    Overworld = 0,
    Nether = 1,
    TheEnd = 2,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::DimensionNameIds>(::bedrock::protocol::DimensionNameIds value) noexcept {
    switch (value) {
        case ::bedrock::protocol::DimensionNameIds::Overworld: return "minecraft:overworld";
        case ::bedrock::protocol::DimensionNameIds::Nether: return "minecraft:nether";
        case ::bedrock::protocol::DimensionNameIds::TheEnd: return "minecraft:the_end";
        default: return invalid_tag;
    }
}

}
