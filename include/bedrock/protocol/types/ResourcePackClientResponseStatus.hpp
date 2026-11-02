#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class ResourcePackClientResponseStatus : uint16_t {
    Refused = 0,
    SendPacks = 1,
    HaveAllPacks = 2,
    Completed = 3,
};
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<ResourcePackClientResponseStatus>(ResourcePackClientResponseStatus value) noexcept {
    switch (value) {
        case ResourcePackClientResponseStatus::Refused: return "cancel";
        case ResourcePackClientResponseStatus::SendPacks: return "downloading";
        case ResourcePackClientResponseStatus::HaveAllPacks: return "downloadingfinished";
        case ResourcePackClientResponseStatus::Completed: return "resourcepackstackfinished";
        default: return invalid_tag;
    }
}

}
