#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>

#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
namespace bedrock::protocol {


class PrimitiveShapePyramidPayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 7;

    float width;
    std::optional<float> depth;
    float height;

    PrimitiveShapePyramidPayload() = default;
    virtual ~PrimitiveShapePyramidPayload() = default;
    explicit PrimitiveShapePyramidPayload(const float& widthValue, const std::optional<float>& depthValue, const float& heightValue) : width(widthValue), depth(depthValue), height(heightValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        width = stream.readFloat().value_or(0.0f);
        depth = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        height = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(width);
		stream.writeOptional(depth, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeFloat(height);
    }
};

} // namespace bedrock::protocol
