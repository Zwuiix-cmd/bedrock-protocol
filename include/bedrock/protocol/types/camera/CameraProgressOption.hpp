#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraProgressOption {
public:
    float value;
    float time;
    CameraSetInstructionEaseType easeType;

    CameraProgressOption() = default;
    explicit CameraProgressOption(const float& valueValue, const float& timeValue, const CameraSetInstructionEaseType& easeTypeValue) : value(valueValue), time(timeValue), easeType(easeTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readFloat().value_or(0.0f);
        time = stream.readFloat().value_or(0.0f);
        easeType = magic_enum::enum_cast<CameraSetInstructionEaseType>(
            stream.readString().value_or("")
        ).value_or(CameraSetInstructionEaseType::Linear);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(value);
		stream.writeFloat(time);
		stream.writeString(magic_enum::enum_name(easeType));
    }
};

} // namespace bedrock::protocol
