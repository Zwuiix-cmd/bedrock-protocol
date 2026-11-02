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

    enum class Expression : uint8_t {
        Linear = 0,
        Blinking = 1,
    };

    std::shared_ptr<SkinImage> image;
    Type type;
    float frames;
    Type expressionType;

    SkinAnimation() = default;
    explicit SkinAnimation(
        const std::shared_ptr<SkinImage>& imageValue,
        const Type& typeValue,
        const float& framesValue,
        const Type& expressionTypeValue
    ) :
        image(imageValue),
        type(typeValue),
        frames(framesValue),
        expressionType(expressionTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = magic_enum::enum_cast<Type>(stream.readUnsignedVarInt().value_or(0)).value_or(Type::Head);
        frames = stream.readFloat().value_or(0.0f);
        expressionType = magic_enum::enum_cast<Type>(stream.readUnsignedVarInt().value_or(0)).value_or(Type::Head);
        image = ([&]() {
    auto value = std::make_shared<SkinImage>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(magic_enum::enum_integer(type));
        stream.writeFloat(frames);
        stream.writeUnsignedVarInt(magic_enum::enum_integer(expressionType));
        if (image) image->write(stream);
    }
};
