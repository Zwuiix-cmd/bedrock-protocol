#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


class CameraFadeInstructionTime {
public:
    float fadeInTime;
    float stayTime;
    float fadeOutTime;

    CameraFadeInstructionTime() = default;
    explicit CameraFadeInstructionTime(const float& fadeInTimeValue, const float& stayTimeValue, const float& fadeOutTimeValue) : fadeInTime(fadeInTimeValue), stayTime(stayTimeValue), fadeOutTime(fadeOutTimeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        fadeInTime = stream.readFloat().value_or(0.0f);
        stayTime = stream.readFloat().value_or(0.0f);
        fadeOutTime = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(fadeInTime);
		stream.writeFloat(stayTime);
		stream.writeFloat(fadeOutTime);
    }
};

} // namespace bedrock::protocol
