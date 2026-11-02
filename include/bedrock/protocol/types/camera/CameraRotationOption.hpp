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
            stream.readVarInt().value_or(0)
        ).value_or(CameraSetInstructionEaseType::Linear);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVec3(value);
		stream.writeFloat(time);
		stream.writeString(magic_enum::enum_name(easeType));
    }
};
