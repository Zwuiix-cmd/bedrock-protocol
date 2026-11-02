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
        height = stream.readFloat().value_or(0.0f);
        depth = stream.readOptional([&]() {
			return stream.readFloat().value_or(0.0f);
		});
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(width);
		stream.writeOptional(depth, [&](const auto& value) {
					stream.writeFloat(value);
				});
		stream.writeFloat(height);
    }
};
