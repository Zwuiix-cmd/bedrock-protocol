#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class XboxAuthJwtHeader {
public:
    std::string alg;
    std::string kid;
    std::string typ;

    XboxAuthJwtHeader() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        alg = stream.readString().value_or("");
        kid = stream.readString().value_or("");
        typ = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(alg);
        stream.writeString(kid);
        stream.writeString(typ);
    }
};

} // namespace bedrock::protocol
