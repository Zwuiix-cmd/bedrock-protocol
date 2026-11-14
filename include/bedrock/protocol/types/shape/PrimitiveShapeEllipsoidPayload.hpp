#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
namespace bedrock::protocol {


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

} // namespace bedrock::protocol
