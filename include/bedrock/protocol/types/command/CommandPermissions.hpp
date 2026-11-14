#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class CommandPermissions : uint16_t {
    Normal = 0,
    Operator = 1,
    Automation = 2,
    Host = 3,
    Owner = 4,
    Internal = 5,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::CommandPermissions>(::bedrock::protocol::CommandPermissions value) noexcept {
    switch (value) {
        case ::bedrock::protocol::CommandPermissions::Normal: return "any";
        case ::bedrock::protocol::CommandPermissions::Operator: return "gamedirectors";
        case ::bedrock::protocol::CommandPermissions::Automation: return "admin";
        case ::bedrock::protocol::CommandPermissions::Host: return "host";
        case ::bedrock::protocol::CommandPermissions::Owner: return "owner";
        case ::bedrock::protocol::CommandPermissions::Internal: return "internal";
        default: return invalid_tag;
    }
}

}
