#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class MemoryCategoryCounter {
public:
    uint8_t category;
    uint64_t bytes;

    MemoryCategoryCounter() = default;
    explicit MemoryCategoryCounter(const uint8_t& categoryValue, const uint64_t& bytesValue) : category(categoryValue), bytes(bytesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        category = stream.readUnsignedByte().value_or(0);
        bytes = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(category);
		stream.writeUnsignedLong(bytes);
    }
};
