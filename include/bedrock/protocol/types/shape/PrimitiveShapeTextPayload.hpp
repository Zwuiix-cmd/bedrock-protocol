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

class PrimitiveShapeTextPayload : public PrimitiveShapePayload {
public:
    static constexpr int64_t ID = 2;

    std::string text;
    bool useRotation;
    std::optional<uint32_t> backgroundColor;
    float lineGapHeight;
    bool depthTest;
    bool showBackface;
    bool showTextBackface;

    PrimitiveShapeTextPayload() = default;
    virtual ~PrimitiveShapeTextPayload() = default;
    explicit PrimitiveShapeTextPayload(
        const std::string& textValue,
        const bool& useRotationValue,
        const std::optional<uint32_t>& backgroundColorValue,
        const float& lineGapHeightValue,
        const bool& depthTestValue,
        const bool& showBackfaceValue,
        const bool& showTextBackfaceValue
    ) :
        text(textValue),
        useRotation(useRotationValue),
        backgroundColor(backgroundColorValue),
        lineGapHeight(lineGapHeightValue),
        depthTest(depthTestValue),
        showBackface(showBackfaceValue),
        showTextBackface(showTextBackfaceValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        text = stream.readString().value_or("");
        useRotation = stream.readBool().value_or(false);
        lineGapHeight = stream.readFloat().value_or(0.0f);
        depthTest = stream.readBool().value_or(false);
        showBackface = stream.readBool().value_or(false);
        showTextBackface = stream.readBool().value_or(false);
        backgroundColor = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
    }

    void write(BinaryStream& stream) const override {
        stream.writeString(text);
        stream.writeBool(useRotation);
        stream.writeFloat(lineGapHeight);
        stream.writeBool(depthTest);
        stream.writeBool(showBackface);
        stream.writeBool(showTextBackface);
        stream.writeOptional(backgroundColor, [&](const auto& optionalValue) {
            stream.writeUnsignedInt(optionalValue);
        });
    }
};
