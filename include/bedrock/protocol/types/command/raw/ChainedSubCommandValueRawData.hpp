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

class ChainedSubCommandValueRawData {
public:
    uint32_t nameIndex;
    uint32_t type;

    ChainedSubCommandValueRawData() = default;
    explicit ChainedSubCommandValueRawData(const uint32_t& nameIndexValue, const uint32_t& typeValue) : nameIndex(nameIndexValue), type(typeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        nameIndex = stream.readUnsignedVarInt().value_or(0);
        type = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(nameIndex);
		stream.writeUnsignedVarInt(type);
    }
};
