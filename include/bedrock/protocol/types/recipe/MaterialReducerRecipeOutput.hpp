#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class MaterialReducerRecipeOutput {
public:
    int32_t itemId;
    int32_t count;

    MaterialReducerRecipeOutput() = default;
    explicit MaterialReducerRecipeOutput(const int32_t& itemIdValue, const int32_t& countValue) : itemId(itemIdValue), count(countValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        itemId = stream.readVarInt().value_or(0);
        count = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(itemId);
		stream.writeVarInt(count);
    }
};

} // namespace bedrock::protocol
