#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>


class MapDecoration {
public:
    uint8_t icon;
    uint8_t rotation;
    uint8_t xOffset;
    uint8_t yOffset;
    std::string label;
    uint32_t color;

    MapDecoration() = default;
    explicit MapDecoration(
        const uint8_t& iconValue,
        const uint8_t& rotationValue,
        const uint8_t& xOffsetValue,
        const uint8_t& yOffsetValue,
        const std::string& labelValue,
        const uint32_t& colorValue
    ) :
        icon(iconValue),
        rotation(rotationValue),
        xOffset(xOffsetValue),
        yOffset(yOffsetValue),
        label(labelValue),
        color(colorValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        icon = stream.readUnsignedByte().value_or(0);
        rotation = stream.readUnsignedByte().value_or(0);
        xOffset = stream.readUnsignedByte().value_or(0);
        yOffset = stream.readUnsignedByte().value_or(0);
        label = stream.readString().value_or("");
        color = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedByte(icon);
        stream.writeUnsignedByte(rotation);
        stream.writeUnsignedByte(xOffset);
        stream.writeUnsignedByte(yOffset);
        stream.writeString(label);
        stream.writeUnsignedInt(color);
    }
};
