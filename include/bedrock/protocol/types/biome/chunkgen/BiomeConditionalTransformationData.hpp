#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>

class BiomeConditionalTransformationData {
public:
    std::vector<std::shared_ptr<BiomeWeightedData>> weightedBiomes;
    int16_t conditionJSON;
    uint32_t minPassingNeighbors;

    BiomeConditionalTransformationData() = default;
    explicit BiomeConditionalTransformationData(
        const std::vector<std::shared_ptr<BiomeWeightedData>>& weightedBiomesValue,
        const int16_t& conditionJSONValue,
        const uint32_t& minPassingNeighborsValue
    ) :
        weightedBiomes(weightedBiomesValue),
        conditionJSON(conditionJSONValue),
        minPassingNeighbors(minPassingNeighborsValue) {}

    static std::shared_ptr<BiomeConditionalTransformationData> create(ReadOnlyBinaryStream& stream) {
		auto weightedBiomes = std::vector<std::shared_ptr<BiomeWeightedData>>();
		auto weightedBiomesCount = stream.readUnsignedVarInt().value_or(0);
		const bool weightedBiomesCountAvailable = stream.canReadCollection(weightedBiomesCount);
		for (std::size_t i = 0; i < weightedBiomesCount && weightedBiomesCountAvailable && !stream.hasOverflowed(); ++i) {
		    weightedBiomes.push_back(stream.readObject<BiomeWeightedData>());
		}
		auto conditionJSON = stream.readShort().value_or(0);
		auto minPassingNeighbors = stream.readUnsignedInt().value_or(0);
		return std::make_shared<BiomeConditionalTransformationData>(weightedBiomes, static_cast<int32_t>(conditionJSON), static_cast<int32_t>(minPassingNeighbors));
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(weightedBiomes.size());
		for (const auto& biome : weightedBiomes) {
			biome->write(stream);
		}
		stream.writeShort(conditionJSON);
		stream.writeUnsignedInt(minPassingNeighbors);
    }
};
