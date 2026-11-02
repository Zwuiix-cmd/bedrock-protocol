#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>


class ClientDataAnimationFrame {
public:
    int32_t ImageHeight;
    int32_t ImageWidth;
    float Frames;
    int32_t Type;
    std::string Image;
    int32_t AnimationExpression;

    ClientDataAnimationFrame() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        ImageHeight = stream.readInt().value_or(0);
        ImageWidth = stream.readInt().value_or(0);
        Frames = stream.readFloat().value_or(0.0f);
        Type = stream.readInt().value_or(0);
        Image = stream.readString().value_or("");
        AnimationExpression = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(ImageHeight);
        stream.writeInt(ImageWidth);
        stream.writeFloat(Frames);
        stream.writeInt(Type);
        stream.writeString(Image);
        stream.writeInt(AnimationExpression);
    }
};
