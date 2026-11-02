#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class BiomeWeightedTemperatureData {
public:
    int32_t temperature;
    uint32_t weight;

    BiomeWeightedTemperatureData() = default;
    explicit BiomeWeightedTemperatureData(const int32_t& temperatureValue, const uint32_t& weightValue) : temperature(temperatureValue), weight(weightValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        temperature = stream.readVarInt().value_or(0);
        weight = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(temperature);
		stream.writeUnsignedInt(weight);
    }
};

} // namespace bedrock::protocol
