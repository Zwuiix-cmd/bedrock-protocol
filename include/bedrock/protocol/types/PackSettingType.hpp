#pragma once

#include <cstdint>
#include <string_view>

enum class PackSettingType : uint8_t {
    Float = 0,
    Bool = 1,
    String = 2,
};
