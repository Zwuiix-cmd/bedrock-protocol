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

class NetworkPermissions {
public:
    bool disableClientSounds;

    NetworkPermissions() = default;
    explicit NetworkPermissions(const bool& disableClientSoundsValue) : disableClientSounds(disableClientSoundsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        disableClientSounds = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeBool(disableClientSounds);
    }
};
