#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
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

    virtual void read(ReadOnlyBinaryStream& stream) {
        weightedBiomes = stream.readObjectList<BiomeWeightedData>();
        conditionJSON = stream.readShort().value_or(0);
        minPassingNeighbors = stream.readUnsignedInt().value_or(0);
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
