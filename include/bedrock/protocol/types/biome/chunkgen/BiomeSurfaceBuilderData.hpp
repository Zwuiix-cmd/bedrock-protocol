#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>
namespace bedrock::protocol {


class BiomeSurfaceBuilderData {
public:
    std::optional<std::shared_ptr<BiomeSurfaceMaterialData>> surfaceMaterial;
    bool defaultOverworldSurface;
    bool swampSurface;
    bool frozenOceanSurface;
    bool theEndSurface;
    std::optional<std::shared_ptr<BiomeMesaSurfaceData>> mesaSurface;
    std::optional<std::shared_ptr<BiomeCappedSurfaceData>> cappedSurface;
    std::optional<std::shared_ptr<BiomeNoiseGradientSurfaceData>> noiseGradientSurface;

    BiomeSurfaceBuilderData() = default;
    explicit BiomeSurfaceBuilderData(
        const std::optional<std::shared_ptr<BiomeSurfaceMaterialData>>& surfaceMaterialValue,
        const bool& defaultOverworldSurfaceValue,
        const bool& swampSurfaceValue,
        const bool& frozenOceanSurfaceValue,
        const bool& theEndSurfaceValue,
        const std::optional<std::shared_ptr<BiomeMesaSurfaceData>>& mesaSurfaceValue,
        const std::optional<std::shared_ptr<BiomeCappedSurfaceData>>& cappedSurfaceValue,
        const std::optional<std::shared_ptr<BiomeNoiseGradientSurfaceData>>& noiseGradientSurfaceValue
    ) :
        surfaceMaterial(surfaceMaterialValue),
        defaultOverworldSurface(defaultOverworldSurfaceValue),
        swampSurface(swampSurfaceValue),
        frozenOceanSurface(frozenOceanSurfaceValue),
        theEndSurface(theEndSurfaceValue),
        mesaSurface(mesaSurfaceValue),
        cappedSurface(cappedSurfaceValue),
        noiseGradientSurface(noiseGradientSurfaceValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        surfaceMaterial = stream.readOptional([&]() {
            return stream.readObject<BiomeSurfaceMaterialData>();
        });
        defaultOverworldSurface = stream.readBool().value_or(false);
        swampSurface = stream.readBool().value_or(false);
        frozenOceanSurface = stream.readBool().value_or(false);
        theEndSurface = stream.readBool().value_or(false);
        mesaSurface = stream.readOptional([&]() {
            return stream.readObject<BiomeMesaSurfaceData>();
        });
        cappedSurface = stream.readOptional([&]() {
            return stream.readObject<BiomeCappedSurfaceData>();
        });
        noiseGradientSurface = stream.readOptional([&]() {
            return stream.readObject<BiomeNoiseGradientSurfaceData>();
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(surfaceMaterial, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeBool(defaultOverworldSurface);
		stream.writeBool(swampSurface);
		stream.writeBool(frozenOceanSurface);
		stream.writeBool(theEndSurface);
		stream.writeOptional(mesaSurface, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(cappedSurface, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(noiseGradientSurface, [&](const auto& v) {
			v->write(stream);
		});
    }
};

} // namespace bedrock::protocol
