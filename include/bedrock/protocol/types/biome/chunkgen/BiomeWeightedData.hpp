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

class BiomeWeightedData {
public:
    int16_t biome;
    uint32_t weight;

    BiomeWeightedData() = default;
    explicit BiomeWeightedData(const int16_t& biomeValue, const uint32_t& weightValue) : biome(biomeValue), weight(weightValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biome = stream.readShort().value_or(0);
        weight = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeShort(biome);
		stream.writeUnsignedInt(weight);
    }
};
