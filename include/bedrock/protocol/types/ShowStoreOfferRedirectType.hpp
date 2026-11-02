#pragma once

#include <cstdint>
#include <string_view>

enum class ShowStoreOfferRedirectType : uint8_t {
    Marketplace = 0,
    DressingRoom = 1,
    ThirdPartyServerPage = 2,
};
