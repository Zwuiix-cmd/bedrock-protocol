#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class Enchant {
public:
    uint8_t id;
    uint8_t level;

    Enchant() = default;
    explicit Enchant(const uint8_t& idValue, const uint8_t& levelValue) : id(idValue), level(levelValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readUnsignedByte().value_or(0);
        level = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(id);
		stream.writeUnsignedByte(level);
    }
};
