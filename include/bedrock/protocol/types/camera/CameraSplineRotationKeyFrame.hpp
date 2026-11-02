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
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>

class CameraSplineRotationKeyFrame {
public:
    glm::vec3 rotation;
    float time;
    std::optional<CameraSetInstructionEaseType> easeType;

    CameraSplineRotationKeyFrame() = default;
    explicit CameraSplineRotationKeyFrame(
        const glm::vec3& rotationValue,
        const float& timeValue,
        const std::optional<CameraSetInstructionEaseType>& easeTypeValue
    ) :
        rotation(rotationValue),
        time(timeValue),
        easeType(easeTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        rotation = stream.readVec3().value_or(glm::vec3(0.0f));
        time = stream.readFloat().value_or(0.0f);
        easeType = stream.readOptional([&]() {
            return static_cast<CameraSetInstructionEaseType>(stream.readVarInt().value_or(0));
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVec3(rotation);
		stream.writeFloat(time);
		stream.writeOptional(easeType, [&](const auto& v) {
		            stream.writeString(magic_enum::enum_name(v));
		        });
    }
};
