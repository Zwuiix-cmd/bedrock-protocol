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

class Enchant {
public:
    uint32_t id;
    uint8_t level;

    Enchant() = default;
    explicit Enchant(const uint32_t& idValue, const uint8_t& levelValue) : id(idValue), level(levelValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readUnsignedVarInt().value_or(0);
        level = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(id);
		stream.writeUnsignedByte(level);
    }
};
