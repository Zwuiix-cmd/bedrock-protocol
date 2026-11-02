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
    enum class Y : uint8_t {
        IndexMultiplier = 128,
    };

    uint32_t color;
    int32_t x;
    Y y;

    MapInfoRequestPacketClientPixel() = default;
    explicit MapInfoRequestPacketClientPixel(const uint32_t& colorValue, const int32_t& xValue, const Y& yValue) : color(colorValue), x(xValue), y(yValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        color = stream.readUnsignedInt().value_or(0);
        x = stream.readInt().value_or(0);
        y = magic_enum::enum_cast<Y>(stream.readInt().value_or(0)).value_or(magic_enum::enum_value<Y>(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(static_cast<uint32_t>(color));
		stream.writeUnsignedShort((x + (magic_enum::enum_integer(y) * magic_enum::enum_integer(Y::IndexMultiplier))));
    }
};
