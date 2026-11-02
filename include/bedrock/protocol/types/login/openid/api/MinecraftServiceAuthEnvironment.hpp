#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class MinecraftServiceAuthEnvironment {
public:
    std::string serviceUri;

    MinecraftServiceAuthEnvironment() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        serviceUri = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(serviceUri);
    }
};

} // namespace bedrock::protocol
