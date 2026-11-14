#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraRotationOption {
public:
    glm::vec3 value;
    float time;
    CameraSetInstructionEaseType easeType;

    CameraRotationOption() = default;
    explicit CameraRotationOption(
        const glm::vec3& valueValue,
        const float& timeValue,
        const CameraSetInstructionEaseType& easeTypeValue
    ) :
        value(valueValue),
        time(timeValue),
        easeType(easeTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readVec3().value_or(glm::vec3(0.0f));
        time = stream.readFloat().value_or(0.0f);
        easeType = magic_enum::enum_cast<CameraSetInstructionEaseType>(
            stream.readString().value_or("")
        ).value_or(CameraSetInstructionEaseType::Linear);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVec3(value);
		stream.writeFloat(time);
		stream.writeString(magic_enum::enum_name(easeType));
    }
};

} // namespace bedrock::protocol
