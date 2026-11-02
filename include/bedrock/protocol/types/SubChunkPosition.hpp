#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class SubChunkPosition {
public:
    int32_t x;
    int32_t y;
    int32_t z;

    SubChunkPosition() = default;
    explicit SubChunkPosition(const int32_t& xValue, const int32_t& yValue, const int32_t& zValue) : x(xValue), y(yValue), z(zValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        x = stream.readVarInt().value_or(0);
        y = stream.readVarInt().value_or(0);
        z = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(x);
		stream.writeVarInt(y);
		stream.writeVarInt(z);
    }
};

} // namespace bedrock::protocol
