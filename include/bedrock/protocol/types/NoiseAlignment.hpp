#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/NoiseAlignmentType.hpp>
namespace bedrock::protocol {


class NoiseAlignment {
public:
    NoiseAlignmentType type;
    uint32_t value;

    NoiseAlignment() = default;
    explicit NoiseAlignment(const NoiseAlignmentType& typeValue, const uint32_t& valueValue) : type(typeValue), value(valueValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        type = magic_enum::enum_cast<NoiseAlignmentType>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(NoiseAlignmentType::MinLocalTransitionEnd);
        value = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(type));
		stream.writeUnsignedVarInt(value);
    }
};

} // namespace bedrock::protocol
