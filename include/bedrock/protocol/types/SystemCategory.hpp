#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class SystemCategory {
public:
    std::string categoryName;
    uint64_t systemIndex;

    SystemCategory() = default;
    explicit SystemCategory(const std::string& categoryNameValue, const uint64_t& systemIndexValue) : categoryName(categoryNameValue), systemIndex(systemIndexValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        categoryName = stream.readString().value_or("");
        systemIndex = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(categoryName);
		stream.writeUnsignedLong(systemIndex);
    }
};

} // namespace bedrock::protocol
