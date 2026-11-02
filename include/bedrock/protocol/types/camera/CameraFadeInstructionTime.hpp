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
