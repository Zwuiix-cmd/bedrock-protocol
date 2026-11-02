#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.hpp>
namespace bedrock::protocol {


class BiomeOverworldGenRulesData {
public:
    std::vector<std::shared_ptr<BiomeWeightedData>> hillTransformations;
    std::vector<std::shared_ptr<BiomeWeightedData>> mutateTransformations;
    std::vector<std::shared_ptr<BiomeWeightedData>> riverTransformations;
    std::vector<std::shared_ptr<BiomeWeightedData>> shoreTransformations;
    std::vector<std::shared_ptr<BiomeConditionalTransformationData>> preHillsEdges;
    std::vector<std::shared_ptr<BiomeConditionalTransformationData>> postShoreEdges;
    std::vector<std::shared_ptr<BiomeWeightedTemperatureData>> climates;

    BiomeOverworldGenRulesData() = default;
    explicit BiomeOverworldGenRulesData(
        const std::vector<std::shared_ptr<BiomeWeightedData>>& hillTransformationsValue,
        const std::vector<std::shared_ptr<BiomeWeightedData>>& mutateTransformationsValue,
        const std::vector<std::shared_ptr<BiomeWeightedData>>& riverTransformationsValue,
        const std::vector<std::shared_ptr<BiomeWeightedData>>& shoreTransformationsValue,
        const std::vector<std::shared_ptr<BiomeConditionalTransformationData>>& preHillsEdgesValue,
        const std::vector<std::shared_ptr<BiomeConditionalTransformationData>>& postShoreEdgesValue,
        const std::vector<std::shared_ptr<BiomeWeightedTemperatureData>>& climatesValue
    ) :
        hillTransformations(hillTransformationsValue),
        mutateTransformations(mutateTransformationsValue),
        riverTransformations(riverTransformationsValue),
        shoreTransformations(shoreTransformationsValue),
        preHillsEdges(preHillsEdgesValue),
        postShoreEdges(postShoreEdgesValue),
        climates(climatesValue) {}

    static std::shared_ptr<BiomeOverworldGenRulesData> create(ReadOnlyBinaryStream& stream) {
		auto hillTransformations = std::vector<std::shared_ptr<BiomeWeightedData>>();
		auto hillTransformationsCount = stream.readUnsignedVarInt().value_or(0);
		const bool hillTransformationsCountAvailable = stream.canReadCollection(hillTransformationsCount);
		for (std::size_t i = 0; i < hillTransformationsCount && hillTransformationsCountAvailable && !stream.hasOverflowed(); ++i) {
		    hillTransformations.push_back(stream.readObject<BiomeWeightedData>());
		}
		auto mutateTransformations = std::vector<std::shared_ptr<BiomeWeightedData>>();
		auto mutateTransformationsCount = stream.readUnsignedVarInt().value_or(0);
		const bool mutateTransformationsCountAvailable = stream.canReadCollection(mutateTransformationsCount);
		for (std::size_t i = 0; i < mutateTransformationsCount && mutateTransformationsCountAvailable && !stream.hasOverflowed(); ++i) {
		    mutateTransformations.push_back(stream.readObject<BiomeWeightedData>());
		}
		auto riverTransformations = std::vector<std::shared_ptr<BiomeWeightedData>>();
		auto riverTransformationsCount = stream.readUnsignedVarInt().value_or(0);
		const bool riverTransformationsCountAvailable = stream.canReadCollection(riverTransformationsCount);
		for (std::size_t i = 0; i < riverTransformationsCount && riverTransformationsCountAvailable && !stream.hasOverflowed(); ++i) {
		    riverTransformations.push_back(stream.readObject<BiomeWeightedData>());
		}
		auto shoreTransformations = std::vector<std::shared_ptr<BiomeWeightedData>>();
		auto shoreTransformationsCount = stream.readUnsignedVarInt().value_or(0);
		const bool shoreTransformationsCountAvailable = stream.canReadCollection(shoreTransformationsCount);
		for (std::size_t i = 0; i < shoreTransformationsCount && shoreTransformationsCountAvailable && !stream.hasOverflowed(); ++i) {
		    shoreTransformations.push_back(stream.readObject<BiomeWeightedData>());
		}
		auto preHillsEdges = std::vector<std::shared_ptr<BiomeConditionalTransformationData>>();
		auto preHillsEdgesCount = stream.readUnsignedVarInt().value_or(0);
		const bool preHillsEdgesCountAvailable = stream.canReadCollection(preHillsEdgesCount);
		for (std::size_t i = 0; i < preHillsEdgesCount && preHillsEdgesCountAvailable && !stream.hasOverflowed(); ++i) {
		    preHillsEdges.push_back(stream.readObject<BiomeConditionalTransformationData>());
		}
		auto postShoreEdges = std::vector<std::shared_ptr<BiomeConditionalTransformationData>>();
		auto postShoreEdgesCount = stream.readUnsignedVarInt().value_or(0);
		const bool postShoreEdgesCountAvailable = stream.canReadCollection(postShoreEdgesCount);
		for (std::size_t i = 0; i < postShoreEdgesCount && postShoreEdgesCountAvailable && !stream.hasOverflowed(); ++i) {
		    postShoreEdges.push_back(stream.readObject<BiomeConditionalTransformationData>());
		}
		auto climates = std::vector<std::shared_ptr<BiomeWeightedTemperatureData>>();
		auto climatesCount = stream.readUnsignedVarInt().value_or(0);
		const bool climatesCountAvailable = stream.canReadCollection(climatesCount);
		for (std::size_t i = 0; i < climatesCount && climatesCountAvailable && !stream.hasOverflowed(); ++i) {
		    climates.push_back(stream.readObject<BiomeWeightedTemperatureData>());
		}
		return std::make_shared<BiomeOverworldGenRulesData>(hillTransformations, mutateTransformations, riverTransformations, shoreTransformations, preHillsEdges, postShoreEdges, climates);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(hillTransformations.size());
		for (const auto& transformation : hillTransformations) {
			transformation->write(stream);
		}
		stream.writeUnsignedVarInt(mutateTransformations.size());
		for (const auto& transformation : mutateTransformations) {
			transformation->write(stream);
		}
		stream.writeUnsignedVarInt(riverTransformations.size());
		for (const auto& transformation : riverTransformations) {
			transformation->write(stream);
		}
		stream.writeUnsignedVarInt(shoreTransformations.size());
		for (const auto& transformation : shoreTransformations) {
			transformation->write(stream);
		}
		stream.writeUnsignedVarInt(preHillsEdges.size());
		for (const auto& edge : preHillsEdges) {
			edge->write(stream);
		}
		stream.writeUnsignedVarInt(postShoreEdges.size());
		for (const auto& edge : postShoreEdges) {
			edge->write(stream);
		}
		stream.writeUnsignedVarInt(climates.size());
		for (const auto& climate : climates) {
			climate->write(stream);
		}
    }
};

} // namespace bedrock::protocol
