#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class AuthServiceOpenIdConfiguration {
public:
    std::string issuer;
    std::string jwks_uri;

    AuthServiceOpenIdConfiguration() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        issuer = stream.readString().value_or("");
        jwks_uri = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(issuer);
        stream.writeString(jwks_uri);
    }
};

} // namespace bedrock::protocol
