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

class BiomeNoiseBlockSpecifier {
public:
    std::string noise;
    float threshold;
    float min;
    float max;
    uint32_t block;

    BiomeNoiseBlockSpecifier() = default;
    explicit BiomeNoiseBlockSpecifier(
        const std::string& noiseValue,
        const float& thresholdValue,
        const float& minValue,
        const float& maxValue,
        const uint32_t& blockValue
    ) :
        noise(noiseValue),
        threshold(thresholdValue),
        min(minValue),
        max(maxValue),
        block(blockValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        noise = stream.readString().value_or("");
        threshold = stream.readFloat().value_or(0.0f);
        min = stream.readFloat().value_or(0.0f);
        max = stream.readFloat().value_or(0.0f);
        block = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(noise);
		stream.writeFloat(threshold);
		stream.writeFloat(min);
		stream.writeFloat(max);
		stream.writeUnsignedInt(block);
    }
};
