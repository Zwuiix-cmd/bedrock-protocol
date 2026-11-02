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

class MinecraftServiceAuthEnvironment {
public:
    std::string serviceUri;

    MinecraftServiceAuthEnvironment() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        serviceUri = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(serviceUri);
    }
};
