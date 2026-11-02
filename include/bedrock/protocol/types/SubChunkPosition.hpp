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
