#pragma once

#include <cstdint>
#include <limits>
#include <magic_enum/magic_enum.hpp>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <bedrock/protocol/Forward.hpp>

class CommandPermissions {
public:
    static constexpr int64_t NORMAL = 0;
    static constexpr int64_t OPERATOR = 1;
    static constexpr int64_t AUTOMATION = 2;
    static constexpr int64_t HOST = 3;
    static constexpr int64_t OWNER = 4;
    static constexpr int64_t INTERNAL = 5;


    CommandPermissions() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
