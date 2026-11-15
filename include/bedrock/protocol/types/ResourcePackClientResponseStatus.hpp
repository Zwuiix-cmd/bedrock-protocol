#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ResourcePackClientResponseStatus : uint16_t {
    Refused = 0,
    SendPacks = 1,
    HaveAllPacks = 2,
    Completed = 3,
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::ResourcePackClientResponseStatus>(::bedrock::protocol::ResourcePackClientResponseStatus value) noexcept {
    switch (value) {
        case ::bedrock::protocol::ResourcePackClientResponseStatus::Refused: return "cancel";
        case ::bedrock::protocol::ResourcePackClientResponseStatus::SendPacks: return "downloading";
        case ::bedrock::protocol::ResourcePackClientResponseStatus::HaveAllPacks: return "downloadingfinished";
        case ::bedrock::protocol::ResourcePackClientResponseStatus::Completed: return "resourcepackstackfinished";
        default: return invalid_tag;
    }
}

}
