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
