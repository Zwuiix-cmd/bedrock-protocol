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
#include <bedrock/protocol/types/login/AuthenticationType.hpp>
#include <bedrock/protocol/types/login/AuthenticationType.hpp>

class AuthenticationInfo {
public:
    ::AuthenticationType AuthenticationType;
    std::string Certificate;
    std::string Token;

    AuthenticationInfo() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        AuthenticationType = static_cast<::AuthenticationType>(stream.readInt().value_or(0));
        Certificate = stream.readString().value_or("");
        Token = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(static_cast<std::underlying_type_t<::AuthenticationType>>(AuthenticationType));
        stream.writeString(Certificate);
        stream.writeString(Token);
    }
};
