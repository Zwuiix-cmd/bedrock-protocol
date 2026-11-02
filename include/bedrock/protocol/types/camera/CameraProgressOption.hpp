#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


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

} // namespace bedrock::protocol
