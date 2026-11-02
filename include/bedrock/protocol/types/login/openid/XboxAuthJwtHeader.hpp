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
