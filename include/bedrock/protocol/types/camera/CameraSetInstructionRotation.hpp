#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>


class CameraSetInstructionRotation {
public:
    float pitch;
    float yaw;

    CameraSetInstructionRotation() = default;
    explicit CameraSetInstructionRotation(const float& pitchValue, const float& yawValue) : pitch(pitchValue), yaw(yawValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        pitch = stream.readFloat().value_or(0.0f);
        yaw = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(pitch);
		stream.writeFloat(yaw);
    }
};
