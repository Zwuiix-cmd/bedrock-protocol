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

class PrimitiveShapeArrowPayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 1;

    std::optional<glm::vec3> lineEndLocation;
    std::optional<float> arrowHeadLength;
    std::optional<float> arrowHeadRadius;
    std::optional<int32_t> segments;

    PrimitiveShapeArrowPayload() = default;
    virtual ~PrimitiveShapeArrowPayload() = default;
    explicit PrimitiveShapeArrowPayload(
        const std::optional<glm::vec3>& lineEndLocationValue,
        const std::optional<float>& arrowHeadLengthValue,
        const std::optional<float>& arrowHeadRadiusValue,
        const std::optional<int32_t>& segmentsValue
    ) :
        lineEndLocation(lineEndLocationValue),
        arrowHeadLength(arrowHeadLengthValue),
        arrowHeadRadius(arrowHeadRadiusValue),
        segments(segmentsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        lineEndLocation = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        arrowHeadLength = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        arrowHeadRadius = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        segments = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeOptional(lineEndLocation, [&](const auto& value) {
		            stream.writeVec3(value);
		        });
		stream.writeOptional(arrowHeadLength, [&](const auto& value) {
		            stream.writeFloat(value);
		        });
		stream.writeOptional(arrowHeadRadius, [&](const auto& value) {
		            stream.writeFloat(value);
		        });
		stream.writeOptional(segments, [&](const auto& value) {
		            stream.writeUnsignedByte(value);
		        });
    }
};
