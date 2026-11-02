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

class MapInfoRequestPacketClientPixel {
public:
    static constexpr int64_t Y_INDEX_MULTIPLIER = 128;

    uint32_t color;
    int32_t x;
    int32_t y;

    MapInfoRequestPacketClientPixel() = default;
    explicit MapInfoRequestPacketClientPixel(const uint32_t& colorValue, const int32_t& xValue, const int32_t& yValue) : color(colorValue), x(xValue), y(yValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        color = stream.readUnsignedInt().value_or(0);
        x = stream.readInt().value_or(0);
        y = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(static_cast<uint32_t>(color));
		stream.writeUnsignedShort((x + (y * Y_INDEX_MULTIPLIER)));
    }
};
