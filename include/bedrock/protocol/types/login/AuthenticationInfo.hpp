#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>


class AuthenticationInfo {
public:
    int32_t AuthenticationType;
    std::string Certificate;
    std::string Token;

    AuthenticationInfo() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        AuthenticationType = stream.readInt().value_or(0);
        Certificate = stream.readString().value_or("");
        Token = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(AuthenticationType);
        stream.writeString(Certificate);
        stream.writeString(Token);
    }
};
