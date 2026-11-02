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
#include <bedrock/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseGradientSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>

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
        defaultOverworldSurface = stream.readBool().value_or(false);
        swampSurface = stream.readBool().value_or(false);
        frozenOceanSurface = stream.readBool().value_or(false);
        theEndSurface = stream.readBool().value_or(false);
        surfaceMaterial = stream.readOptional([&]() {
            auto value = std::make_shared<BiomeSurfaceMaterialData>();
            value->read(stream);
            return value;
        });
        mesaSurface = stream.readOptional([&]() {
            auto value = std::make_shared<BiomeMesaSurfaceData>();
            value->read(stream);
            return value;
        });
        cappedSurface = stream.readOptional([&]() {
            auto value = std::make_shared<BiomeCappedSurfaceData>();
            value->read(stream);
            return value;
        });
        noiseGradientSurface = stream.readOptional([&]() {
            auto value = std::make_shared<BiomeNoiseGradientSurfaceData>();
            value->read(stream);
            return value;
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
