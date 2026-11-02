#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/camera/CameraProgressOption.hpp>
#include <bedrock/protocol/types/camera/CameraRotationOption.hpp>
namespace bedrock::protocol {


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

    static std::shared_ptr<CameraSplineInstruction> create(ReadOnlyBinaryStream& stream) {
		auto totalTime = stream.readFloat().value_or(0.0f);
		auto easeType = stream.readUnsignedByte().value_or(0);
		auto curve = std::vector<glm::vec3>();
		auto curveCount = stream.readUnsignedVarInt().value_or(0);
		    const bool curveCountAvailable = stream.canReadCollection(curveCount);
		for (std::size_t i = 0; (i < curveCount) && !stream.hasOverflowed() && curveCountAvailable && !stream.hasOverflowed(); ++i) {
			curve.push_back(stream.readVec3().value_or(glm::vec3(0.0f)));
		}
		auto progressKeyFrames = std::vector<std::shared_ptr<CameraProgressOption>>();
		auto progressKeyFrameCount = stream.readUnsignedVarInt().value_or(0);
		    const bool progressKeyFrameCountAvailable = stream.canReadCollection(progressKeyFrameCount);
		for (std::size_t i = 0; (i < progressKeyFrameCount) && !stream.hasOverflowed() && progressKeyFrameCountAvailable && !stream.hasOverflowed(); ++i) {
			progressKeyFrames.push_back(stream.readObject<CameraProgressOption>());
		}
		auto rotationOptions = std::vector<std::shared_ptr<CameraRotationOption>>();
		auto rotationOptionCount = stream.readUnsignedVarInt().value_or(0);
		    const bool rotationOptionCountAvailable = stream.canReadCollection(rotationOptionCount);
		for (std::size_t i = 0; (i < rotationOptionCount) && !stream.hasOverflowed() && rotationOptionCountAvailable && !stream.hasOverflowed(); ++i) {
			rotationOptions.push_back(stream.readObject<CameraRotationOption>());
		}
		auto splineIdentifier = stream.readString().value_or("");
		auto loadFromJson = stream.readBool().value_or(false);
		return std::make_shared<CameraSplineInstruction>(totalTime, static_cast<int32_t>(easeType), curve, progressKeyFrames, rotationOptions, splineIdentifier, loadFromJson);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
