#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/camera/CameraFadeInstructionColor.hpp>
#include <bedrock/protocol/types/camera/CameraFadeInstructionTime.hpp>

class CameraFadeInstruction {
public:
    std::optional<std::shared_ptr<CameraFadeInstructionTime>> time;
    std::optional<std::shared_ptr<CameraFadeInstructionColor>> color;

    CameraFadeInstruction() = default;
    explicit CameraFadeInstruction(
        const std::optional<std::shared_ptr<CameraFadeInstructionTime>>& timeValue,
        const std::optional<std::shared_ptr<CameraFadeInstructionColor>>& colorValue
    ) :
        time(timeValue),
        color(colorValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        time = stream.readOptional([&]() {
            return stream.readObject<CameraFadeInstructionTime>();
        });
        color = stream.readOptional([&]() {
            return stream.readObject<CameraFadeInstructionColor>();
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(time, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(color, [&](const auto& v) {
			v->write(stream);
		});
    }
};
