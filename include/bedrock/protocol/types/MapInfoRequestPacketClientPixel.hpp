#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

namespace bedrock::protocol {


class MapInfoRequestPacketClientPixel {
public:
    static constexpr int64_t Y_INDEX_MULTIPLIER = 128;

    uint32_t color;
    int32_t x;
    int32_t y;

    MapInfoRequestPacketClientPixel() = default;
    explicit MapInfoRequestPacketClientPixel(const uint32_t& colorValue, const int32_t& xValue, const int32_t& yValue) : color(colorValue), x(xValue), y(yValue) {}

    static std::shared_ptr<MapInfoRequestPacketClientPixel> create(ReadOnlyBinaryStream& stream) {
		auto color = stream.readUnsignedInt().value_or(0);
		auto index = stream.readUnsignedShort().value_or(0);
		auto x = (index % Y_INDEX_MULTIPLIER);
		auto y = (index / Y_INDEX_MULTIPLIER);
		return std::make_shared<MapInfoRequestPacketClientPixel>(static_cast<uint32_t>(color), static_cast<int32_t>(x), static_cast<int32_t>(y));
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(static_cast<uint32_t>(color));
		stream.writeUnsignedShort((x + (y * Y_INDEX_MULTIPLIER)));
    }
};

} // namespace bedrock::protocol
