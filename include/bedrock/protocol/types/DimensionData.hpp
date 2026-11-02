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

class DimensionData {
public:
    int32_t maxHeight;
    int32_t minHeight;
    int32_t generator;

    DimensionData() = default;
    explicit DimensionData(const int32_t& maxHeightValue, const int32_t& minHeightValue, const int32_t& generatorValue) : maxHeight(maxHeightValue), minHeight(minHeightValue), generator(generatorValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        maxHeight = stream.readVarInt().value_or(0);
        minHeight = stream.readVarInt().value_or(0);
        generator = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(maxHeight);
		stream.writeVarInt(minHeight);
		stream.writeVarInt(generator);
    }
};
