#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
namespace bedrock::protocol {


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

} // namespace bedrock::protocol
