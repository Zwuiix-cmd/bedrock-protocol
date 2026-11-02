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

class AuthServiceKey {
public:
    std::string kty;
    std::string use;
    std::string kid;
    std::string x5t;
    std::string n;
    std::string e;

    AuthServiceKey() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        kty = stream.readString().value_or("");
        use = stream.readString().value_or("");
        kid = stream.readString().value_or("");
        x5t = stream.readString().value_or("");
        n = stream.readString().value_or("");
        e = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(kty);
        stream.writeString(use);
        stream.writeString(kid);
        stream.writeString(x5t);
        stream.writeString(n);
        stream.writeString(e);
    }
};
