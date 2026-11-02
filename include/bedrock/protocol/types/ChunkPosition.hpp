#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class ChunkPosition {
public:
    int32_t x;
    int32_t z;

    ChunkPosition() = default;
    explicit ChunkPosition(const int32_t& xValue, const int32_t& zValue) : x(xValue), z(zValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        x = stream.readVarInt().value_or(0);
        z = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(x);
		stream.writeVarInt(z);
    }
};

} // namespace bedrock::protocol
