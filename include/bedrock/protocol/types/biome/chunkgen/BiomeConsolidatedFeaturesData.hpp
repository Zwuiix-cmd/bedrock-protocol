#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>

class BiomeConsolidatedFeaturesData {
public:
    std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>> features;

    BiomeConsolidatedFeaturesData() = default;
    explicit BiomeConsolidatedFeaturesData(const std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>& featuresValue) : features(featuresValue) {}

    static std::shared_ptr<BiomeConsolidatedFeaturesData> create(ReadOnlyBinaryStream& stream) {
		auto features = std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>();
		auto featuresCount = stream.readUnsignedVarInt().value_or(0);
		if (featuresCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < featuresCount; ++i) {
		    features.push_back(stream.readObject<BiomeConsolidatedFeatureData>());
		}
		return std::make_shared<BiomeConsolidatedFeaturesData>(features);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(features.size());
		for (const auto& feature : features) {
			feature->write(stream);
		}
    }
};
