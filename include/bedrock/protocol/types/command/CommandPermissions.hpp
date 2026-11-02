#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class CommandPermissions : uint16_t {
    Normal = 0,
    Operator = 1,
    Automation = 2,
    Host = 3,
    Owner = 4,
    Internal = 5,
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CommandPermissions>(CommandPermissions value) noexcept {
    switch (value) {
        case CommandPermissions::Normal: return "any";
        case CommandPermissions::Operator: return "gamedirectors";
        case CommandPermissions::Automation: return "admin";
        case CommandPermissions::Host: return "host";
        case CommandPermissions::Owner: return "owner";
        case CommandPermissions::Internal: return "internal";
        default: return invalid_tag;
    }
}

}
