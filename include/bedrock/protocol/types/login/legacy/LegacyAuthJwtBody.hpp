#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/login/JwtBodyRfc7519.hpp>

class LegacyAuthJwtBody : public JwtBodyRfc7519 {
public:
    std::string identityPublicKey;

    LegacyAuthJwtBody() = default;
    virtual ~LegacyAuthJwtBody() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        identityPublicKey = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(identityPublicKey);
    }
};
