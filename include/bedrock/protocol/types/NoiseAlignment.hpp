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
#include <bedrock/protocol/types/NoiseAlignmentType.hpp>
#include <bedrock/protocol/types/NoiseAlignmentType.hpp>
#include <bedrock/protocol/types/NoiseAlignmentType.hpp>

class NoiseAlignment {
public:
    NoiseAlignmentType type;
    uint32_t value;

    NoiseAlignment() = default;
    explicit NoiseAlignment(const NoiseAlignmentType& typeValue, const uint32_t& valueValue) : type(typeValue), value(valueValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        value = stream.readUnsignedVarInt().value_or(0);
        type = magic_enum::enum_cast<NoiseAlignmentType>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<NoiseAlignmentType>(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(type));
		stream.writeUnsignedVarInt(value);
    }
};
