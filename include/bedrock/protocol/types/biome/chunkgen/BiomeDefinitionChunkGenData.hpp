#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeClimateData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeaturesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeLegacyWorldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMountainParamsData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeMultinoiseGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeOverworldGenRulesData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeReplacementData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceBuilderData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeSurfaceMaterialAdjustmentData.hpp>
namespace bedrock::protocol {


class BiomeDefinitionChunkGenData {
public:
    std::optional<std::shared_ptr<BiomeClimateData>> climate;
    std::optional<std::shared_ptr<BiomeConsolidatedFeaturesData>> consolidatedFeatures;
    std::optional<std::shared_ptr<BiomeMountainParamsData>> mountainParams;
    std::optional<std::shared_ptr<BiomeSurfaceMaterialAdjustmentData>> surfaceMaterialAdjustment;
    std::optional<std::shared_ptr<BiomeOverworldGenRulesData>> overworldGenRules;
    std::optional<std::shared_ptr<BiomeMultinoiseGenRulesData>> multinoiseGenRules;
    std::optional<std::shared_ptr<BiomeLegacyWorldGenRulesData>> legacyWorldGenRules;
    std::optional<std::vector<std::shared_ptr<BiomeReplacementData>>> replacementsData;
    std::optional<uint8_t> villageType;
    std::optional<std::shared_ptr<BiomeSurfaceBuilderData>> surfaceBuilderData;
    std::optional<std::shared_ptr<BiomeSurfaceBuilderData>> subSurfaceBuilderData;

    BiomeDefinitionChunkGenData() = default;
    explicit BiomeDefinitionChunkGenData(
        const std::optional<std::shared_ptr<BiomeClimateData>>& climateValue,
        const std::optional<std::shared_ptr<BiomeConsolidatedFeaturesData>>& consolidatedFeaturesValue,
        const std::optional<std::shared_ptr<BiomeMountainParamsData>>& mountainParamsValue,
        const std::optional<std::shared_ptr<BiomeSurfaceMaterialAdjustmentData>>& surfaceMaterialAdjustmentValue,
        const std::optional<std::shared_ptr<BiomeOverworldGenRulesData>>& overworldGenRulesValue,
        const std::optional<std::shared_ptr<BiomeMultinoiseGenRulesData>>& multinoiseGenRulesValue,
        const std::optional<std::shared_ptr<BiomeLegacyWorldGenRulesData>>& legacyWorldGenRulesValue,
        const std::optional<std::vector<std::shared_ptr<BiomeReplacementData>>>& replacementsDataValue,
        const std::optional<uint8_t>& villageTypeValue,
        const std::optional<std::shared_ptr<BiomeSurfaceBuilderData>>& surfaceBuilderDataValue,
        const std::optional<std::shared_ptr<BiomeSurfaceBuilderData>>& subSurfaceBuilderDataValue
    ) :
        climate(climateValue),
        consolidatedFeatures(consolidatedFeaturesValue),
        mountainParams(mountainParamsValue),
        surfaceMaterialAdjustment(surfaceMaterialAdjustmentValue),
        overworldGenRules(overworldGenRulesValue),
        multinoiseGenRules(multinoiseGenRulesValue),
        legacyWorldGenRules(legacyWorldGenRulesValue),
        replacementsData(replacementsDataValue),
        villageType(villageTypeValue),
        surfaceBuilderData(surfaceBuilderDataValue),
        subSurfaceBuilderData(subSurfaceBuilderDataValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        climate = stream.readOptional([&]() {
            return stream.readObject<BiomeClimateData>();
        });
        consolidatedFeatures = stream.readOptional([&]() {
            return stream.readObject<BiomeConsolidatedFeaturesData>();
        });
        mountainParams = stream.readOptional([&]() {
            return stream.readObject<BiomeMountainParamsData>();
        });
        surfaceMaterialAdjustment = stream.readOptional([&]() {
            return stream.readObject<BiomeSurfaceMaterialAdjustmentData>();
        });
        overworldGenRules = stream.readOptional([&]() {
            return stream.readObject<BiomeOverworldGenRulesData>();
        });
        multinoiseGenRules = stream.readOptional([&]() {
            return stream.readObject<BiomeMultinoiseGenRulesData>();
        });
        legacyWorldGenRules = stream.readOptional([&]() {
            return stream.readObject<BiomeLegacyWorldGenRulesData>();
        });
        replacementsData = stream.readObjectList<BiomeReplacementData>();
        villageType = stream.readOptional([&]() {
            return stream.readUnsignedByte().value_or(0);
        });
        surfaceBuilderData = stream.readOptional([&]() {
            return stream.readObject<BiomeSurfaceBuilderData>();
        });
        subSurfaceBuilderData = stream.readOptional([&]() {
            return stream.readObject<BiomeSurfaceBuilderData>();
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
		stream.writeOptional(surfaceBuilderData, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(subSurfaceBuilderData, [&](const auto& v) {
			v->write(stream);
		});
    }
};

} // namespace bedrock::protocol
