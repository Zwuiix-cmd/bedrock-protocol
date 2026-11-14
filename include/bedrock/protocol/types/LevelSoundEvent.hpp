#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class LevelSoundEvent : uint16_t {
#include <bedrock/protocol/types/LevelSoundEventCases0.inc>
#include <bedrock/protocol/types/LevelSoundEventCases1.inc>
#include <bedrock/protocol/types/LevelSoundEventCases2.inc>
};

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::LevelSoundEvent>(::bedrock::protocol::LevelSoundEvent value) noexcept {
    switch (value) {
#include <bedrock/protocol/types/LevelSoundEventNames0.inc>
#include <bedrock/protocol/types/LevelSoundEventNames1.inc>
#include <bedrock/protocol/types/LevelSoundEventNames2.inc>
        default: return invalid_tag;
    }
}

}
