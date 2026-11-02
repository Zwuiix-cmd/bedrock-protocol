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
