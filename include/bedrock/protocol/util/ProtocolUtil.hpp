#pragma once

#include <type_traits>

namespace bedrock::protocol::util {

template <typename Enum>
    requires std::is_enum_v<Enum>
[[nodiscard]] constexpr std::underlying_type_t<Enum> toUnderlying(Enum value) noexcept {
    return static_cast<std::underlying_type_t<Enum>>(value);
}

template <typename Enum, typename T>
    requires std::is_enum_v<Enum>
[[nodiscard]] constexpr Enum fromUnderlying(T value) noexcept {
    return static_cast<Enum>(static_cast<std::underlying_type_t<Enum>>(value));
}

template <typename To, typename From>
[[nodiscard]] constexpr To narrow(From value) noexcept {
    return static_cast<To>(value);
}

} // namespace bedrock::protocol::util
