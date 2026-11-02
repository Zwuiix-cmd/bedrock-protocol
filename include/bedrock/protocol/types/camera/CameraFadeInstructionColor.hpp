#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>


class CameraFadeInstructionColor {
public:
    float red;
    float green;
    float blue;

    CameraFadeInstructionColor() = default;
    explicit CameraFadeInstructionColor(const float& redValue, const float& greenValue, const float& blueValue) : red(redValue), green(greenValue), blue(blueValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        red = stream.readFloat().value_or(0.0f);
        green = stream.readFloat().value_or(0.0f);
        blue = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(red);
		stream.writeFloat(green);
		stream.writeFloat(blue);
    }
};
