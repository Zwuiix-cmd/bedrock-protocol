#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>
#include <string>

#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
namespace bedrock::protocol {


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
        backgroundColor = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        lineGapHeight = stream.readFloat().value_or(0.0f);
        depthTest = stream.readBool().value_or(false);
        showBackface = stream.readBool().value_or(false);
        showTextBackface = stream.readBool().value_or(false);
    }

    void write(BinaryStream& stream) const override {
        stream.writeString(text);
        stream.writeBool(useRotation);
        stream.writeOptional(backgroundColor, [&](const auto& optionalValue) {
            stream.writeUnsignedInt(optionalValue);
        });
        stream.writeFloat(lineGapHeight);
        stream.writeBool(depthTest);
        stream.writeBool(showBackface);
        stream.writeBool(showTextBackface);
    }
};

} // namespace bedrock::protocol
