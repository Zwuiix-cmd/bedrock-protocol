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

class BiomeMultinoiseGenRulesData {
public:
    float temperature;
    float humidity;
    float altitude;
    float weirdness;
    float weight;

    BiomeMultinoiseGenRulesData() = default;
    explicit BiomeMultinoiseGenRulesData(
        const float& temperatureValue,
        const float& humidityValue,
        const float& altitudeValue,
        const float& weirdnessValue,
        const float& weightValue
    ) :
        temperature(temperatureValue),
        humidity(humidityValue),
        altitude(altitudeValue),
        weirdness(weirdnessValue),
        weight(weightValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        temperature = stream.readFloat().value_or(0.0f);
        humidity = stream.readFloat().value_or(0.0f);
        altitude = stream.readFloat().value_or(0.0f);
        weirdness = stream.readFloat().value_or(0.0f);
        weight = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(temperature);
		stream.writeFloat(humidity);
		stream.writeFloat(altitude);
		stream.writeFloat(weirdness);
		stream.writeFloat(weight);
    }
};
