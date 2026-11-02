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
