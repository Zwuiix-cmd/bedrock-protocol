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
#include <bedrock/protocol/types/camera/CameraFadeInstructionColor.hpp>
#include <bedrock/protocol/types/camera/CameraFadeInstructionTime.hpp>
#include <bedrock/protocol/types/camera/CameraFadeInstructionColor.hpp>
#include <bedrock/protocol/types/camera/CameraFadeInstructionTime.hpp>
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
            return ([&]() {
            auto value = std::make_shared<CameraFadeInstructionTime>();
            value->read(stream);
            return value;
            }());
        });
        color = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<CameraFadeInstructionColor>();
            value->read(stream);
            return value;
            }());
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
