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
