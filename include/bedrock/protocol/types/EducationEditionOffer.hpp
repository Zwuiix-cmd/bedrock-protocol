#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class EducationEditionOffer : uint8_t {
    None = 0,
    EverywhereExceptChina = 1,
    China = 2,
};
