#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/camera/CameraProgressOption.hpp>
#include <bedrock/protocol/types/camera/CameraRotationOption.hpp>

class CameraSplineInstruction {
public:
    float totalTime;
    uint8_t easeType;
    std::vector<glm::vec3> curve;
    std::vector<std::shared_ptr<CameraProgressOption>> progressKeyFrames;
    std::vector<std::shared_ptr<CameraRotationOption>> rotationOptions;
    std::string splineIdentifier;
    bool loadFromJson;

    CameraSplineInstruction() = default;
    explicit CameraSplineInstruction(
        const float& totalTimeValue,
        const uint8_t& easeTypeValue,
        const std::vector<glm::vec3>& curveValue,
        const std::vector<std::shared_ptr<CameraProgressOption>>& progressKeyFramesValue,
        const std::vector<std::shared_ptr<CameraRotationOption>>& rotationOptionsValue,
        const std::string& splineIdentifierValue,
        const bool& loadFromJsonValue
    ) :
        totalTime(totalTimeValue),
        easeType(easeTypeValue),
        curve(curveValue),
        progressKeyFrames(progressKeyFramesValue),
        rotationOptions(rotationOptionsValue),
        splineIdentifier(splineIdentifierValue),
        loadFromJson(loadFromJsonValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        totalTime = stream.readFloat().value_or(0.0f);
        easeType = stream.readUnsignedByte().value_or(0);
        curve = stream.readList([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        progressKeyFrames = stream.readObjectList<CameraProgressOption>();
        rotationOptions = stream.readObjectList<CameraRotationOption>();
        splineIdentifier = stream.readString().value_or("");
        loadFromJson = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(totalTime);
		stream.writeUnsignedByte(easeType);
		stream.writeUnsignedVarInt(curve.size());
		for (const auto& point : curve) {
			stream.writeVec3(point);
		}
		stream.writeUnsignedVarInt(progressKeyFrames.size());
		for (const auto& keyFrame : progressKeyFrames) {
			keyFrame->write(stream);
		}
		stream.writeUnsignedVarInt(rotationOptions.size());
		for (const auto& option : rotationOptions) {
			option->write(stream);
		}
		stream.writeString(splineIdentifier);
		stream.writeBool(loadFromJson);
    }
};
