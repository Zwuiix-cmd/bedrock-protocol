#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraRotationOption {
public:
    std::string easeType;
    glm::vec3 value;
    float time;

    CameraRotationOption() = default;
    explicit CameraRotationOption(const glm::vec3& valueValue, const float& timeValue, const std::string& easeTypeValue) : value(valueValue), time(timeValue), easeType(easeTypeValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readVec3().value_or(glm::vec3(0.0f));
        time = stream.readFloat().value_or(0.0f);
        easeType = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVec3(value);
		stream.writeFloat(time);
		stream.writeString(easeType);
    }
};

} // namespace bedrock::protocol
