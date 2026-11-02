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

class CameraFovInstruction {
public:
    float fieldOfView;
    float easeTime;
    CameraSetInstructionEaseType easeType;
    bool clear;

    CameraFovInstruction() = default;
    explicit CameraFovInstruction(
        const float& fieldOfViewValue,
        const float& easeTimeValue,
        const CameraSetInstructionEaseType& easeTypeValue,
        const bool& clearValue
    ) :
        fieldOfView(fieldOfViewValue),
        easeTime(easeTimeValue),
        easeType(easeTypeValue),
        clear(clearValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        fieldOfView = stream.readFloat().value_or(0.0f);
        easeTime = stream.readFloat().value_or(0.0f);
        clear = stream.readBool().value_or(false);
        easeType = magic_enum::enum_cast<CameraSetInstructionEaseType>(stream.readVarInt().value_or(0)).value_or(CameraSetInstructionEaseType::Linear);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(fieldOfView);
		stream.writeFloat(easeTime);
		stream.writeString(magic_enum::enum_name(easeType));
		stream.writeBool(clear);
    }
};
