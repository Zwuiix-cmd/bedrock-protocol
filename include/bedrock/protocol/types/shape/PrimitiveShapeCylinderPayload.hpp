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

class PrimitiveShapeCylinderPayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 6;

    glm::vec2 radiusX;
    glm::vec2 radiusZ;
    float height;
    uint8_t segments;

    PrimitiveShapeCylinderPayload() = default;
    virtual ~PrimitiveShapeCylinderPayload() = default;
    explicit PrimitiveShapeCylinderPayload(
        const glm::vec2& radiusXValue,
        const glm::vec2& radiusZValue,
        const float& heightValue,
        const uint8_t& segmentsValue
    ) :
        radiusX(radiusXValue),
        radiusZ(radiusZValue),
        height(heightValue),
        segments(segmentsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        radiusX = stream.readVec2().value_or(glm::vec2(0.0f));
        radiusZ = stream.readVec2().value_or(glm::vec2(0.0f));
        height = stream.readFloat().value_or(0.0f);
        segments = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVec2(radiusX);
		stream.writeVec2(radiusZ);
		stream.writeFloat(height);
		stream.writeUnsignedByte(segments);
    }
};
