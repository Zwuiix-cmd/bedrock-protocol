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
#include <bedrock/protocol/types/camera/CameraAimAssistTargetMode.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistTargetMode.hpp>
#include <bedrock/protocol/types/camera/CameraAimAssistTargetMode.hpp>

class CameraPresetAimAssist {
public:
    std::optional<std::string> presetId;
    std::optional<CameraAimAssistTargetMode> targetMode;
    std::optional<glm::vec2> viewAngle;
    std::optional<float> distance;

    CameraPresetAimAssist() = default;
    explicit CameraPresetAimAssist(
        const std::optional<std::string>& presetIdValue,
        const std::optional<CameraAimAssistTargetMode>& targetModeValue,
        const std::optional<glm::vec2>& viewAngleValue,
        const std::optional<float>& distanceValue
    ) :
        presetId(presetIdValue),
        targetMode(targetModeValue),
        viewAngle(viewAngleValue),
        distance(distanceValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        presetId = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        targetMode = stream.readOptional([&]() {
            return static_cast<CameraAimAssistTargetMode>(stream.readVarInt().value_or(0));
        });
        viewAngle = stream.readOptional([&]() {
            return stream.readVec2().value_or(glm::vec2(0.0f));
        });
        distance = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(presetId, [&](const auto& value) {
		            stream.writeString(value);
		        });
		stream.writeOptional(targetMode, [&](const auto& v) {
		            stream.writeUnsignedByte(static_cast<std::underlying_type_t<CameraAimAssistTargetMode>>(v));
		        });
		stream.writeOptional(viewAngle, [&](const auto& value) {
		            stream.writeVec2(value);
		        });
		stream.writeOptional(distance, [&](const auto& value) {
		            stream.writeFloat(value);
		        });
    }
};
