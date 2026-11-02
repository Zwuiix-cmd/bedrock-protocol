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
