#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class AuthenticationType : uint8_t {
    Full = 0,
    Guest = 1,
    SelfSigned = 2,
};
