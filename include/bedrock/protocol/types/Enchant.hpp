#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class Enchant {
public:
    uint32_t id;
    uint8_t level;

    Enchant() = default;
    explicit Enchant(const uint32_t& idValue, const uint8_t& levelValue) : id(idValue), level(levelValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readUnsignedVarInt().value_or(0);
        level = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(id);
		stream.writeUnsignedByte(level);
    }
};

} // namespace bedrock::protocol
