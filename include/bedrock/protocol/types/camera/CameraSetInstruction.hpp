#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/camera/CameraSetInstructionEase.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionRotation.hpp>

class CameraSetInstruction {
public:
    uint32_t preset;
    std::optional<std::shared_ptr<CameraSetInstructionEase>> ease;
    std::optional<glm::vec3> cameraPosition;
    std::optional<std::shared_ptr<CameraSetInstructionRotation>> rotation;
    std::optional<glm::vec3> facingPosition;
    std::optional<glm::vec2> viewOffset;
    std::optional<glm::vec3> entityOffset;
    std::optional<bool> default_;
    bool ignoreStartingValuesComponent;

    CameraSetInstruction() = default;
    explicit CameraSetInstruction(
        const uint32_t& presetValue,
        const std::optional<std::shared_ptr<CameraSetInstructionEase>>& easeValue,
        const std::optional<glm::vec3>& cameraPositionValue,
        const std::optional<std::shared_ptr<CameraSetInstructionRotation>>& rotationValue,
        const std::optional<glm::vec3>& facingPositionValue,
        const std::optional<glm::vec2>& viewOffsetValue,
        const std::optional<glm::vec3>& entityOffsetValue,
        const std::optional<bool>& default_Value,
        const bool& ignoreStartingValuesComponentValue
    ) :
        preset(presetValue),
        ease(easeValue),
        cameraPosition(cameraPositionValue),
        rotation(rotationValue),
        facingPosition(facingPositionValue),
        viewOffset(viewOffsetValue),
        entityOffset(entityOffsetValue),
        default_(default_Value),
        ignoreStartingValuesComponent(ignoreStartingValuesComponentValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        preset = stream.readUnsignedInt().value_or(0);
        ignoreStartingValuesComponent = stream.readBool().value_or(false);
        ease = stream.readOptional([&]() {
            return stream.readObject<CameraSetInstructionEase>();
        });
        cameraPosition = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        rotation = stream.readOptional([&]() {
            return stream.readObject<CameraSetInstructionRotation>();
        });
        facingPosition = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        viewOffset = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
        entityOffset = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        default_ = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(preset);
		stream.writeOptional(ease, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(cameraPosition, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeOptional(rotation, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(facingPosition, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeOptional(viewOffset, [&](const auto& value) {
			stream.writeVec2(value);
		});
		stream.writeOptional(entityOffset, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeOptional(default_, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeBool(ignoreStartingValuesComponent);
    }
};
