#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


class BiomeClimateData {
public:
    float temperature;
    float downfall;
    float snowAccumulationMin;
    float snowAccumulationMax;

    BiomeClimateData() = default;
    explicit BiomeClimateData(
        const float& temperatureValue,
        const float& downfallValue,
        const float& snowAccumulationMinValue,
        const float& snowAccumulationMaxValue
    ) :
        temperature(temperatureValue),
        downfall(downfallValue),
        snowAccumulationMin(snowAccumulationMinValue),
        snowAccumulationMax(snowAccumulationMaxValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        temperature = stream.readFloat().value_or(0.0f);
        downfall = stream.readFloat().value_or(0.0f);
        snowAccumulationMin = stream.readFloat().value_or(0.0f);
        snowAccumulationMax = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(temperature);
		stream.writeFloat(downfall);
		stream.writeFloat(snowAccumulationMin);
		stream.writeFloat(snowAccumulationMax);
    }
};

} // namespace bedrock::protocol
