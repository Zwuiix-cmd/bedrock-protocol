#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class SelfSignedJwtHeader {
public:
    std::string alg;
    std::string x5u;
    std::string x5t;

    SelfSignedJwtHeader() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        alg = stream.readString().value_or("");
        x5u = stream.readString().value_or("");
        x5t = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(alg);
        stream.writeString(x5u);
        stream.writeString(x5t);
    }
};

} // namespace bedrock::protocol
