#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeLayerSettingsWeightType : uint8_t {
    Float = 0,
    String = 1,
};

} // namespace bedrock::protocol
