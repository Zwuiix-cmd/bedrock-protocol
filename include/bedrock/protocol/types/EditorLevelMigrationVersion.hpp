#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class EditorLevelMigrationVersion : uint8_t {
    Legacy = 0,
    EditorAchievements = 1,
};

} // namespace bedrock::protocol
