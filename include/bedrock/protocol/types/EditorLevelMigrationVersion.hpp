#pragma once

#include <cstdint>
#include <string_view>

enum class EditorLevelMigrationVersion : uint8_t {
    Legacy = 0,
    EditorAchievements = 1,
};
