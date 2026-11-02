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
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>

class CameraProgressOption {
public:
    std::string easeType;
    float value;
    float time;

    CameraProgressOption() = default;
    explicit CameraProgressOption(const float& valueValue, const float& timeValue, const std::string& easeTypeValue) : value(valueValue), time(timeValue), easeType(easeTypeValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readFloat().value_or(0.0f);
        time = stream.readFloat().value_or(0.0f);
        easeType = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(value);
		stream.writeFloat(time);
		stream.writeString(easeType);
    }
};
