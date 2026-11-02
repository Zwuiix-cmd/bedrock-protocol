#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class ShowStoreOfferRedirectType : uint8_t {
    Marketplace = 0,
    DressingRoom = 1,
    ThirdPartyServerPage = 2,
};

} // namespace bedrock::protocol
