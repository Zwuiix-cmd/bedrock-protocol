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
