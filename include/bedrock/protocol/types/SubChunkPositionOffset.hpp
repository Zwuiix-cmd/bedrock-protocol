#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class SubChunkPositionOffset {
public:
    int8_t xOffset;
    int8_t yOffset;
    int8_t zOffset;

    SubChunkPositionOffset() = default;
    explicit SubChunkPositionOffset(const int8_t& xOffsetValue, const int8_t& yOffsetValue, const int8_t& zOffsetValue) : xOffset(xOffsetValue), yOffset(yOffsetValue), zOffset(zOffsetValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        xOffset = stream.readByte().value_or(0);
        yOffset = stream.readByte().value_or(0);
        zOffset = stream.readByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeByte(xOffset);
        stream.writeByte(yOffset);
        stream.writeByte(zOffset);
    }
};

} // namespace bedrock::protocol
