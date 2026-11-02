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
