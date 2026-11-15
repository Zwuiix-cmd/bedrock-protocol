#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class DimensionData {
public:
    int32_t minimumY;
    int32_t heightRange;
    int32_t generator;
    int32_t dimensionType;
    std::string packId;
    std::string defaultBiome;
    int32_t cloudHeight;
    bool renderClouds;

    DimensionData() = default;
    explicit DimensionData(
        const int32_t& minimumYValue,
        const int32_t& heightRangeValue,
        const int32_t& generatorValue,
        const int32_t& dimensionTypeValue,
        const std::string& packIdValue,
        const std::string& defaultBiomeValue,
        const int32_t& cloudHeightValue,
        const bool& renderCloudsValue
    ) :
        minimumY(minimumYValue),
        heightRange(heightRangeValue),
        generator(generatorValue),
        dimensionType(dimensionTypeValue),
        packId(packIdValue),
        defaultBiome(defaultBiomeValue),
        cloudHeight(cloudHeightValue),
        renderClouds(renderCloudsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        minimumY = stream.readVarInt().value_or(0);
        heightRange = stream.readVarInt().value_or(0);
        generator = stream.readVarInt().value_or(0);
        dimensionType = stream.readVarInt().value_or(0);
        packId = stream.readUUID().value_or("");
        defaultBiome = stream.readString().value_or("");
        cloudHeight = stream.readVarInt().value_or(0);
        renderClouds = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(minimumY);
		stream.writeVarInt(heightRange);
		stream.writeVarInt(generator);
		stream.writeVarInt(dimensionType);
		stream.writeUUID(packId);
		stream.writeString(defaultBiome);
		stream.writeVarInt(cloudHeight);
		stream.writeBool(renderClouds);
    }
};

} // namespace bedrock::protocol
