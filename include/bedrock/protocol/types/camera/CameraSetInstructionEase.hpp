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

class CameraSetInstructionEase {
public:
    CameraSetInstructionEaseType type;
    float duration;

    CameraSetInstructionEase() = default;
    explicit CameraSetInstructionEase(const CameraSetInstructionEaseType& typeValue, const float& durationValue) : type(typeValue), duration(durationValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        duration = stream.readFloat().value_or(0.0f);
        type = magic_enum::enum_cast<CameraSetInstructionEaseType>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<CameraSetInstructionEaseType>(0));
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeFloat(duration);
        stream.writeVarInt(magic_enum::enum_integer(type));
    }
};
