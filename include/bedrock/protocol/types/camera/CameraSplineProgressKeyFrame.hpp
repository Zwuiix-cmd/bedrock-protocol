#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <optional>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraSplineProgressKeyFrame {
public:
    float progress;
    float time;
    std::optional<CameraSetInstructionEaseType> easeType;

    CameraSplineProgressKeyFrame() = default;
    explicit CameraSplineProgressKeyFrame(
        const float& progressValue,
        const float& timeValue,
        const std::optional<CameraSetInstructionEaseType>& easeTypeValue
    ) :
        progress(progressValue),
        time(timeValue),
        easeType(easeTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        progress = stream.readFloat().value_or(0.0f);
        time = stream.readFloat().value_or(0.0f);
        easeType = stream.readOptional([&]() {
            return magic_enum::enum_cast<CameraSetInstructionEaseType>(
            stream.readString().value_or("")
        ).value_or(CameraSetInstructionEaseType::Linear);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(progress);
		stream.writeFloat(time);
		stream.writeOptional(easeType, [&](const auto& v) {
			stream.writeString(magic_enum::enum_name(v));
		});
    }
};

} // namespace bedrock::protocol
