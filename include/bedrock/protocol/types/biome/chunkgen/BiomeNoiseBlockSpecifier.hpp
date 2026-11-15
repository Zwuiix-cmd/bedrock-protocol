#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


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

} // namespace bedrock::protocol
