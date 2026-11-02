#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <string>

#include <bedrock/protocol/types/login/AuthenticationType.hpp>
namespace bedrock::protocol {


class AuthenticationInfo {
public:
    ::bedrock::protocol::AuthenticationType AuthenticationType;
    std::string Certificate;
    std::string Token;

    AuthenticationInfo() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        AuthenticationType = magic_enum::enum_cast<::bedrock::protocol::AuthenticationType>(
            stream.readInt().value_or(0)
        ).value_or(bedrock::protocol::AuthenticationType::Full);
        Certificate = stream.readString().value_or("");
        Token = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(magic_enum::enum_integer(AuthenticationType));
        stream.writeString(Certificate);
        stream.writeString(Token);
    }
};

} // namespace bedrock::protocol
