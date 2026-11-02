#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>
#include <vector>

namespace bedrock::protocol {


class LegacyAuthChain {
public:
    std::vector<std::string> chain;

    LegacyAuthChain() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        chain = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(chain, [&](const auto& item) {
            stream.writeString(item);
        });
    }
};

} // namespace bedrock::protocol
