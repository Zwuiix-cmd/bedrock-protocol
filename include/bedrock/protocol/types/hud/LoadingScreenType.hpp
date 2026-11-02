#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class LoadingScreenType : uint8_t {
    Unknown = 0,
    StartLoadingScreen = 1,
    StopLoadingScreen = 2,
};
