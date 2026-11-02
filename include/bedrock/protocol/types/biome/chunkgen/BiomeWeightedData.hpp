#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
