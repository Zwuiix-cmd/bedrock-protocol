#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class CameraSetInstructionEase {
public:
    uint8_t type;
    float duration;

    CameraSetInstructionEase() = default;
    explicit CameraSetInstructionEase(const uint8_t& typeValue, const float& durationValue) : type(typeValue), duration(durationValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = stream.readUnsignedByte().value_or(0);
        duration = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(type);
		stream.writeFloat(duration);
    }
};

} // namespace bedrock::protocol
