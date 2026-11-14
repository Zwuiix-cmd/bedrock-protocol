#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


class CameraSetInstructionEase {
public:
    CameraSetInstructionEaseType type;
    float duration;

    CameraSetInstructionEase() = default;
    explicit CameraSetInstructionEase(const CameraSetInstructionEaseType& typeValue, const float& durationValue) : type(typeValue), duration(durationValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = magic_enum::enum_cast<CameraSetInstructionEaseType>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(CameraSetInstructionEaseType::Linear);
        duration = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(type));
		stream.writeFloat(duration);
    }
};

} // namespace bedrock::protocol
