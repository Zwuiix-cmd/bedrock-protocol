#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
namespace bedrock::protocol {


class PrimitiveShapeCircleOrSpherePayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 5;

    uint8_t segments;

    PrimitiveShapeCircleOrSpherePayload() = default;
    virtual ~PrimitiveShapeCircleOrSpherePayload() = default;
    explicit PrimitiveShapeCircleOrSpherePayload(const uint8_t& segmentsValue) : segments(segmentsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        segments = stream.readUnsignedByte().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedByte(segments);
    }
};

} // namespace bedrock::protocol
