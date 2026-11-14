#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class DebugMarkerData {
public:
    std::string text;
    glm::vec3 position;
    uint32_t color;
    uint64_t durationMillis;

    DebugMarkerData() = default;
    explicit DebugMarkerData(
        const std::string& textValue,
        const glm::vec3& positionValue,
        const uint32_t& colorValue,
        const uint64_t& durationMillisValue
    ) :
        text(textValue),
        position(positionValue),
        color(colorValue),
        durationMillis(durationMillisValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        text = stream.readString().value_or("");
        position = stream.readVec3().value_or(glm::vec3(0.0f));
        color = stream.readUnsignedInt().value_or(0);
        durationMillis = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(text);
		stream.writeVec3(position);
		stream.writeUnsignedInt(color);
		stream.writeUnsignedLong(durationMillis);
    }
};

} // namespace bedrock::protocol
