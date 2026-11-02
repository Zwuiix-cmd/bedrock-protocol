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
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>

class BiomeElementData {
public:
    float noiseFrequencyScale;
    float noiseLowerBound;
    float noiseUpperBound;
    int32_t heightMinType;
    int16_t heightMin;
    int32_t heightMaxType;
    int16_t heightMax;
    std::shared_ptr<BiomeSurfaceMaterialData> surfaceMaterial;

    BiomeElementData() = default;
    explicit BiomeElementData(
        const float& noiseFrequencyScaleValue,
        const float& noiseLowerBoundValue,
        const float& noiseUpperBoundValue,
        const int32_t& heightMinTypeValue,
        const int16_t& heightMinValue,
        const int32_t& heightMaxTypeValue,
        const int16_t& heightMaxValue,
        const std::shared_ptr<BiomeSurfaceMaterialData>& surfaceMaterialValue
    ) :
        noiseFrequencyScale(noiseFrequencyScaleValue),
        noiseLowerBound(noiseLowerBoundValue),
        noiseUpperBound(noiseUpperBoundValue),
        heightMinType(heightMinTypeValue),
        heightMin(heightMinValue),
        heightMaxType(heightMaxTypeValue),
        heightMax(heightMaxValue),
        surfaceMaterial(surfaceMaterialValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        noiseFrequencyScale = stream.readFloat().value_or(0.0f);
        noiseLowerBound = stream.readFloat().value_or(0.0f);
        noiseUpperBound = stream.readFloat().value_or(0.0f);
        heightMinType = stream.readVarInt().value_or(0);
        heightMin = stream.readShort().value_or(0);
        heightMaxType = stream.readVarInt().value_or(0);
        heightMax = stream.readShort().value_or(0);
        surfaceMaterial = std::make_shared<BiomeSurfaceMaterialData>();
        surfaceMaterial->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(noiseFrequencyScale);
		stream.writeFloat(noiseLowerBound);
		stream.writeFloat(noiseUpperBound);
		stream.writeVarInt(heightMinType);
		stream.writeShort(heightMin);
		stream.writeVarInt(heightMaxType);
		stream.writeShort(heightMax);
		surfaceMaterial->write(stream);
    }
};
