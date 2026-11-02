#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>

class BiomeConsolidatedFeaturesData {
public:
    std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>> features;

    BiomeConsolidatedFeaturesData() = default;
    explicit BiomeConsolidatedFeaturesData(const std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>& featuresValue) : features(featuresValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        features = stream.readObjectList<BiomeConsolidatedFeatureData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(features.size());
		for (const auto& feature : features) {
			feature->write(stream);
		}
    }
};
