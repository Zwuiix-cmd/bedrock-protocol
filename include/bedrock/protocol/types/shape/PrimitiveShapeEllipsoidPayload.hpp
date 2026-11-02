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

class PrimitiveShapeEllipsoidPayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 8;

    glm::vec3 radii;
    uint8_t segmentsPerAxis;

    PrimitiveShapeEllipsoidPayload() = default;
    virtual ~PrimitiveShapeEllipsoidPayload() = default;
    explicit PrimitiveShapeEllipsoidPayload(const glm::vec3& radiiValue, const uint8_t& segmentsPerAxisValue) : radii(radiiValue), segmentsPerAxis(segmentsPerAxisValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        radii = stream.readVec3().value_or(glm::vec3(0.0f));
        segmentsPerAxis = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeVec3(radii);
		stream.writeUnsignedByte(segmentsPerAxis);
    }
};
