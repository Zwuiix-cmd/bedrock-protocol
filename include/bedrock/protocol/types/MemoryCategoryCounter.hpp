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
