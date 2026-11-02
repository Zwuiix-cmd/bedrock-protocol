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
