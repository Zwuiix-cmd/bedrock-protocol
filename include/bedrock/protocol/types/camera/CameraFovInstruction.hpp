#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraFovInstruction {
public:
    std::string easeType;
    float fieldOfView;
    float easeTime;
    bool clear;

    CameraFovInstruction() = default;
    explicit CameraFovInstruction(
        const float& fieldOfViewValue,
        const float& easeTimeValue,
        const std::string& easeTypeValue,
        const bool& clearValue
    ) :
        fieldOfView(fieldOfViewValue),
        easeTime(easeTimeValue),
        easeType(easeTypeValue),
        clear(clearValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        fieldOfView = stream.readFloat().value_or(0.0f);
        easeTime = stream.readFloat().value_or(0.0f);
        easeType = stream.readString().value_or("");
        clear = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(fieldOfView);
		stream.writeFloat(easeTime);
		stream.writeString(easeType);
		stream.writeBool(clear);
    }
};

} // namespace bedrock::protocol
