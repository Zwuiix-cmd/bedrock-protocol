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
#include <bedrock/protocol/types/biome/chunkgen/BiomeClimateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMountainParamsData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeReplacementData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeClimateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMountainParamsData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeReplacementData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeCappedSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeClimateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMesaSurfaceData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMountainParamsData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeReplacementData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialData.hpp>

class BiomeDefinitionChunkGenData {
public:
    std::optional<std::shared_ptr<BiomeClimateData>> climate;
    std::optional<std::shared_ptr<BiomeConsolidatedFeaturesData>> consolidatedFeatures;
    std::optional<std::shared_ptr<BiomeMountainParamsData>> mountainParams;
    std::optional<std::shared_ptr<BiomeSurfaceMaterialAdjustmentData>> surfaceMaterialAdjustment;
    std::optional<std::shared_ptr<BiomeSurfaceMaterialData>> surfaceMaterial;
    bool defaultOverworldSurface;
    bool swampSurface;
    bool frozenOceanSurface;
    bool theEndSurface;
    std::optional<std::shared_ptr<BiomeMesaSurfaceData>> mesaSurface;
    std::optional<std::shared_ptr<BiomeCappedSurfaceData>> cappedSurface;
    std::optional<std::shared_ptr<BiomeOverworldGenRulesData>> overworldGenRules;
    std::optional<std::shared_ptr<BiomeMultinoiseGenRulesData>> multinoiseGenRules;
    std::optional<std::shared_ptr<BiomeLegacyWorldGenRulesData>> legacyWorldGenRules;
    std::optional<std::vector<std::shared_ptr<BiomeReplacementData>>> replacementsData;
    std::optional<int32_t> villageType;

    BiomeDefinitionChunkGenData() = default;
    explicit BiomeDefinitionChunkGenData(
        const std::optional<std::shared_ptr<BiomeClimateData>>& climateValue,
        const std::optional<std::shared_ptr<BiomeConsolidatedFeaturesData>>& consolidatedFeaturesValue,
        const std::optional<std::shared_ptr<BiomeMountainParamsData>>& mountainParamsValue,
        const std::optional<std::shared_ptr<BiomeSurfaceMaterialAdjustmentData>>& surfaceMaterialAdjustmentValue,
        const std::optional<std::shared_ptr<BiomeSurfaceMaterialData>>& surfaceMaterialValue,
        const bool& defaultOverworldSurfaceValue,
        const bool& swampSurfaceValue,
        const bool& frozenOceanSurfaceValue,
        const bool& theEndSurfaceValue,
        const std::optional<std::shared_ptr<BiomeMesaSurfaceData>>& mesaSurfaceValue,
        const std::optional<std::shared_ptr<BiomeCappedSurfaceData>>& cappedSurfaceValue,
        const std::optional<std::shared_ptr<BiomeOverworldGenRulesData>>& overworldGenRulesValue,
        const std::optional<std::shared_ptr<BiomeMultinoiseGenRulesData>>& multinoiseGenRulesValue,
        const std::optional<std::shared_ptr<BiomeLegacyWorldGenRulesData>>& legacyWorldGenRulesValue,
        const std::optional<std::vector<std::shared_ptr<BiomeReplacementData>>>& replacementsDataValue,
        const std::optional<int32_t>& villageTypeValue
    ) :
        climate(climateValue),
        consolidatedFeatures(consolidatedFeaturesValue),
        mountainParams(mountainParamsValue),
        surfaceMaterialAdjustment(surfaceMaterialAdjustmentValue),
        surfaceMaterial(surfaceMaterialValue),
        defaultOverworldSurface(defaultOverworldSurfaceValue),
        swampSurface(swampSurfaceValue),
        frozenOceanSurface(frozenOceanSurfaceValue),
        theEndSurface(theEndSurfaceValue),
        mesaSurface(mesaSurfaceValue),
        cappedSurface(cappedSurfaceValue),
        overworldGenRules(overworldGenRulesValue),
        multinoiseGenRules(multinoiseGenRulesValue),
        legacyWorldGenRules(legacyWorldGenRulesValue),
        replacementsData(replacementsDataValue),
        villageType(villageTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        defaultOverworldSurface = stream.readBool().value_or(false);
        swampSurface = stream.readBool().value_or(false);
        frozenOceanSurface = stream.readBool().value_or(false);
        theEndSurface = stream.readBool().value_or(false);
        climate = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeClimateData>();
            value->read(stream);
            return value;
            }());
        });
        consolidatedFeatures = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeConsolidatedFeaturesData>();
            value->read(stream);
            return value;
            }());
        });
        mountainParams = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeMountainParamsData>();
            value->read(stream);
            return value;
            }());
        });
        surfaceMaterialAdjustment = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeSurfaceMaterialAdjustmentData>();
            value->read(stream);
            return value;
            }());
        });
        surfaceMaterial = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeSurfaceMaterialData>();
            value->read(stream);
            return value;
            }());
        });
        mesaSurface = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeMesaSurfaceData>();
            value->read(stream);
            return value;
            }());
        });
        cappedSurface = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeCappedSurfaceData>();
            value->read(stream);
            return value;
            }());
        });
        overworldGenRules = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeOverworldGenRulesData>();
            value->read(stream);
            return value;
            }());
        });
        multinoiseGenRules = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeMultinoiseGenRulesData>();
            value->read(stream);
            return value;
            }());
        });
        legacyWorldGenRules = stream.readOptional([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeLegacyWorldGenRulesData>();
            value->read(stream);
            return value;
            }());
        });
        replacementsData = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeReplacementData>();
            value->read(stream);
            return value;
            }());
        });
        villageType = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(climate, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(consolidatedFeatures, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(mountainParams, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(surfaceMaterialAdjustment, [&](const auto& v) {
		            v->write(stream);
		        });
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
		stream.writeOptional(overworldGenRules, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(multinoiseGenRules, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(legacyWorldGenRules, [&](const auto& v) {
		            v->write(stream);
		        });
		stream.writeOptional(replacementsData, [&](const auto& v) {
		            	stream.writeUnsignedVarInt(v.size());
		            	for (const auto& item : v) {
				item->write(stream);
			}
		        });
		stream.writeOptional(villageType, [&](const auto& v) {
		            stream.writeUnsignedByte(v);
		        });
    }
};
