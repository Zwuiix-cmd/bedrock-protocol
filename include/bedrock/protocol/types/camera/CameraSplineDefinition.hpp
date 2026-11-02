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
#include <bedrock/protocol/types/camera/CameraSplineProgressKeyFrame.hpp>
#include <bedrock/protocol/types/camera/CameraSplineRotationKeyFrame.hpp>

class CameraSplineDefinition {
public:
    std::string name;
    float totalTime;
    std::string splineType;
    std::vector<glm::vec3> controlPoints;
    std::vector<std::shared_ptr<CameraSplineProgressKeyFrame>> progressKeyFrames;
    std::vector<std::shared_ptr<CameraSplineRotationKeyFrame>> rotationKeyFrames;

    CameraSplineDefinition() = default;
    explicit CameraSplineDefinition(
        const std::string& nameValue,
        const float& totalTimeValue,
        const std::string& splineTypeValue,
        const std::vector<glm::vec3>& controlPointsValue,
        const std::vector<std::shared_ptr<CameraSplineProgressKeyFrame>>& progressKeyFramesValue,
        const std::vector<std::shared_ptr<CameraSplineRotationKeyFrame>>& rotationKeyFramesValue
    ) :
        name(nameValue),
        totalTime(totalTimeValue),
        splineType(splineTypeValue),
        controlPoints(controlPointsValue),
        progressKeyFrames(progressKeyFramesValue),
        rotationKeyFrames(rotationKeyFramesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        totalTime = stream.readFloat().value_or(0.0f);
        splineType = stream.readString().value_or("");
        controlPoints = stream.readList([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        progressKeyFrames = stream.readObjectList<CameraSplineProgressKeyFrame>();
        rotationKeyFrames = stream.readObjectList<CameraSplineRotationKeyFrame>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeFloat(totalTime);
		stream.writeString(splineType);
		stream.writeList(controlPoints, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeList(progressKeyFrames, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeList(rotationKeyFrames, [&](const auto& v) {
			v->write(stream);
		});
    }
};
