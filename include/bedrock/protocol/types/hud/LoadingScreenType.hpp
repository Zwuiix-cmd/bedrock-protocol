#pragma once

#include <cstdint>
#include <string_view>

enum class LoadingScreenType : uint8_t {
    Unknown = 0,
    StartLoadingScreen = 1,
    StopLoadingScreen = 2,
};
