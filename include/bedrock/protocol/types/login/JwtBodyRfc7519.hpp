#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>
#include <variant>

namespace bedrock::protocol {


class JwtBodyRfc7519 {
public:
    std::string iss;
    std::string sub;
    std::monostate aud;
    int32_t exp;
    int32_t nbf;
    int32_t iat;
    std::string jti;

    JwtBodyRfc7519() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        iss = stream.readString().value_or("");
        sub = stream.readString().value_or("");
        aud = std::monostate{};
        exp = stream.readInt().value_or(0);
        nbf = stream.readInt().value_or(0);
        iat = stream.readInt().value_or(0);
        jti = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(iss);
        stream.writeString(sub);
        (void)(aud);
        stream.writeInt(exp);
        stream.writeInt(nbf);
        stream.writeInt(iat);
        stream.writeString(jti);
    }
};

} // namespace bedrock::protocol
