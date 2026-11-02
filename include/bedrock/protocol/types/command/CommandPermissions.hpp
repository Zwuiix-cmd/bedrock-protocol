#pragma once

#include <array>
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

inline constexpr std::array<std::string_view, 6> CommandPermissionsNames = {
#include <bedrock/protocol/types/command/CommandPermissionsNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<CommandPermissions>(CommandPermissions value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < CommandPermissionsNames.size() ? customize_t{CommandPermissionsNames[index]} : invalid_tag;
}

}
