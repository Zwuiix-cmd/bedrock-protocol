#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/skin/SkinImage.hpp>
namespace bedrock::protocol {


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
    Expression expressionType;

    SkinAnimation() = default;
    explicit SkinAnimation(
        const std::shared_ptr<SkinImage>& imageValue,
        const Type& typeValue,
        const float& framesValue,
        const Expression& expressionTypeValue
    ) :
        image(imageValue),
        type(typeValue),
        frames(framesValue),
        expressionType(expressionTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        image = std::make_shared<SkinImage>();
        image->read(stream);
        type = magic_enum::enum_cast<Type>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Type::Head);
        frames = stream.readFloat().value_or(0.0f);
        expressionType = magic_enum::enum_cast<Expression>(
            stream.readUnsignedVarInt().value_or(0)
        ).value_or(Expression::Linear);
    }

    virtual void write(BinaryStream& stream) const {
        image->write(stream);
        stream.writeUnsignedVarInt(magic_enum::enum_integer(type));
        stream.writeFloat(frames);
        stream.writeUnsignedVarInt(magic_enum::enum_integer(expressionType));
    }
};

} // namespace bedrock::protocol
