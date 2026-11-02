#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
