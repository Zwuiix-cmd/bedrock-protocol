#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class ChainedSubCommandValue {
public:
    std::string name;
    int32_t type;

    ChainedSubCommandValue() = default;
    explicit ChainedSubCommandValue(const std::string& nameValue, const int32_t& typeValue) : name(nameValue), type(typeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        type = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(name);
        stream.writeInt(type);
    }
};

} // namespace bedrock::protocol
