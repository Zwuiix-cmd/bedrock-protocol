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
#include <bedrock/protocol/types/ControlScheme.hpp>
#include <bedrock/protocol/types/camera/CameraPresetAimAssist.hpp>

class CameraPreset {
public:
    enum class Audiolistenertype : uint8_t {
        ListenerTypeCamera = 0,
        ListenerTypePlayer = 1,
    };

    std::string name;
    std::string parent;
    std::optional<float> xPosition;
    std::optional<float> yPosition;
    std::optional<float> zPosition;
    std::optional<float> pitch;
    std::optional<float> yaw;
    std::optional<float> rotationSpeed;
    std::optional<bool> snapToTarget;
    std::optional<glm::vec2> horizontalRotationLimit;
    std::optional<glm::vec2> verticalRotationLimit;
    std::optional<bool> continueTargeting;
    std::optional<float> blockListeningRadius;
    std::optional<glm::vec2> viewOffset;
    std::optional<glm::vec3> entityOffset;
    std::optional<float> radius;
    std::optional<float> yawLimitMin;
    std::optional<float> yawLimitMax;
    std::optional<Audiolistenertype> audioListenerType;
    std::optional<bool> playerEffects;
    std::optional<std::shared_ptr<CameraPresetAimAssist>> aimAssist;
    std::optional<ControlScheme> controlScheme;
    bool applyInheritedStartingRotation;
    std::optional<glm::vec2> startingRotation;

    CameraPreset() = default;
    explicit CameraPreset(
        const std::string& nameValue,
        const std::string& parentValue,
        const std::optional<float>& xPositionValue,
        const std::optional<float>& yPositionValue,
        const std::optional<float>& zPositionValue,
        const std::optional<float>& pitchValue,
        const std::optional<float>& yawValue,
        const std::optional<float>& rotationSpeedValue,
        const std::optional<bool>& snapToTargetValue,
        const std::optional<glm::vec2>& horizontalRotationLimitValue,
        const std::optional<glm::vec2>& verticalRotationLimitValue,
        const std::optional<bool>& continueTargetingValue,
        const std::optional<float>& blockListeningRadiusValue,
        const std::optional<glm::vec2>& viewOffsetValue,
        const std::optional<glm::vec3>& entityOffsetValue,
        const std::optional<float>& radiusValue,
        const std::optional<float>& yawLimitMinValue,
        const std::optional<float>& yawLimitMaxValue,
        const std::optional<Audiolistenertype>& audioListenerTypeValue,
        const std::optional<bool>& playerEffectsValue,
        const std::optional<std::shared_ptr<CameraPresetAimAssist>>& aimAssistValue,
        const std::optional<ControlScheme>& controlSchemeValue,
        const bool& applyInheritedStartingRotationValue,
        const std::optional<glm::vec2>& startingRotationValue
    ) :
        name(nameValue),
        parent(parentValue),
        xPosition(xPositionValue),
        yPosition(yPositionValue),
        zPosition(zPositionValue),
        pitch(pitchValue),
        yaw(yawValue),
        rotationSpeed(rotationSpeedValue),
        snapToTarget(snapToTargetValue),
        horizontalRotationLimit(horizontalRotationLimitValue),
        verticalRotationLimit(verticalRotationLimitValue),
        continueTargeting(continueTargetingValue),
        blockListeningRadius(blockListeningRadiusValue),
        viewOffset(viewOffsetValue),
        entityOffset(entityOffsetValue),
        radius(radiusValue),
        yawLimitMin(yawLimitMinValue),
        yawLimitMax(yawLimitMaxValue),
        audioListenerType(audioListenerTypeValue),
        playerEffects(playerEffectsValue),
        aimAssist(aimAssistValue),
        controlScheme(controlSchemeValue),
        applyInheritedStartingRotation(applyInheritedStartingRotationValue),
        startingRotation(startingRotationValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        parent = stream.readString().value_or("");
        applyInheritedStartingRotation = stream.readBool().value_or(false);
        xPosition = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        yPosition = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        zPosition = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        pitch = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        yaw = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        rotationSpeed = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        snapToTarget = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        horizontalRotationLimit = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
        verticalRotationLimit = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
        continueTargeting = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        blockListeningRadius = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        viewOffset = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
        entityOffset = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        radius = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        yawLimitMin = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        yawLimitMax = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        audioListenerType = stream.readOptional([&]() {
            return magic_enum::enum_cast<Audiolistenertype>(
            stream.readInt().value_or(0)
        ).value_or(Audiolistenertype{});
        });
        playerEffects = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        aimAssist = stream.readOptional([&]() {
            auto value = std::make_shared<CameraPresetAimAssist>();
            value->read(stream);
            return value;
        });
        controlScheme = stream.readOptional([&]() {
            return magic_enum::enum_cast<ControlScheme>(
                stream.readVarInt().value_or(0)
            ).value_or(ControlScheme::LockedPlayerRelativeStrafe);
        });
        startingRotation = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeString(parent);
		stream.writeOptional(xPosition, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(yPosition, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(zPosition, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(pitch, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(yaw, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(rotationSpeed, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(snapToTarget, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(horizontalRotationLimit, [&](const auto& value) {
			stream.writeVec2(value);
		});
		stream.writeOptional(verticalRotationLimit, [&](const auto& value) {
			stream.writeVec2(value);
		});
		stream.writeOptional(continueTargeting, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(blockListeningRadius, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(viewOffset, [&](const auto& value) {
			stream.writeVec2(value);
		});
		stream.writeOptional(entityOffset, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeOptional(radius, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(yawLimitMin, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(yawLimitMax, [&](const auto& value) {
			stream.writeFloat(value);
		});
		stream.writeOptional(audioListenerType, [&](const auto& value) {
			stream.writeUnsignedByte(value);
		});
		stream.writeOptional(playerEffects, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(aimAssist, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(controlScheme, [&](const auto& v) {
			stream.writeUnsignedByte(magic_enum::enum_integer(v));
		});
		stream.writeBool(applyInheritedStartingRotation);
		stream.writeOptional(startingRotation, [&](const auto& value) {
			stream.writeVec2(value);
		});
    }
};
