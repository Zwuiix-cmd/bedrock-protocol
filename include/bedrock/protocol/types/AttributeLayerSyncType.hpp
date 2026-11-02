#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class AttributeLayerSyncType : uint8_t {
    UpdateLayers = 0,
    UpdateLayerSettings = 1,
    UpdateEnvironment = 2,
    RemoveEnvironment = 3,
};

} // namespace bedrock::protocol
