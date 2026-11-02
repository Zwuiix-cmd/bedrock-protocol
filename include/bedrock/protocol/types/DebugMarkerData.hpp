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
        durationMillis = stream.readUnsignedLong().value_or(0);
        color = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(text);
		stream.writeVec3(position);
		stream.writeUnsignedInt(static_cast<uint32_t>(color));
		stream.writeUnsignedLong(durationMillis);
    }
};
