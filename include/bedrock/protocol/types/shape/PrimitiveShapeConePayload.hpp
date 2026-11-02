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
#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>

class PrimitiveShapeConePayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 9;

    glm::vec2 radii;
    float height;
    uint8_t segments;

    PrimitiveShapeConePayload() = default;
    virtual ~PrimitiveShapeConePayload() = default;
    explicit PrimitiveShapeConePayload(const glm::vec2& radiiValue, const float& heightValue, const uint8_t& segmentsValue) : radii(radiiValue), height(heightValue), segments(segmentsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        radii = stream.readVec2().value_or(glm::vec2(0.0f));
        height = stream.readFloat().value_or(0.0f);
        segments = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVec2(radii);
		stream.writeFloat(height);
		stream.writeUnsignedByte(segments);
    }
};
