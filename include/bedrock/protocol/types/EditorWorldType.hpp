#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

enum class EditorWorldType : uint8_t {
    NonEditor = 0,
    Project = 1,
    TestLevel = 2,
    RealmsUpload = 3,
};

} // namespace bedrock::protocol
