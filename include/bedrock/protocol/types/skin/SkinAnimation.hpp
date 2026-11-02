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
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>

class SkinAnimation {
public:
    enum class Type : uint8_t {
        Head = 1,
        Body32 = 2,
        Body64 = 3,
    };

    static constexpr int64_t EXPRESSION_LINEAR = 0;
    static constexpr int64_t EXPRESSION_BLINKING = 1;

    std::shared_ptr<SkinImage> image;
    Type type;
    float frames;
    uint32_t expressionType;

    SkinAnimation() = default;
    explicit SkinAnimation(
        const std::shared_ptr<SkinImage>& imageValue,
        const Type& typeValue,
        const float& framesValue,
        const uint32_t& expressionTypeValue
    ) :
        image(imageValue),
        type(typeValue),
        frames(framesValue),
        expressionType(expressionTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = static_cast<Type>(stream.readUnsignedInt().value_or(0));
        frames = stream.readFloat().value_or(0.0f);
        expressionType = stream.readUnsignedInt().value_or(0);
        image = ([&]() {
    auto value = std::make_shared<SkinImage>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedInt(static_cast<std::underlying_type_t<Type>>(type));
        stream.writeFloat(frames);
        stream.writeUnsignedInt(expressionType);
        if (image) image->write(stream);
    }
};
