#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ResourcePackClientResponseStatus : uint16_t {
    Refused = 0,
    SendPacks = 1,
    HaveAllPacks = 2,
    Completed = 3,
};

inline constexpr std::array<std::string_view, 4> ResourcePackClientResponseStatusNames = {
#include <bedrock/protocol/types/ResourcePackClientResponseStatusNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ResourcePackClientResponseStatus>(ResourcePackClientResponseStatus value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < ResourcePackClientResponseStatusNames.size() ? customize_t{ResourcePackClientResponseStatusNames[index]} : invalid_tag;
}

}
