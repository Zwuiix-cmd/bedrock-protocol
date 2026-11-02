#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>
namespace bedrock::protocol {


class BiomeConsolidatedFeaturesData {
public:
    std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>> features;

    BiomeConsolidatedFeaturesData() = default;
    explicit BiomeConsolidatedFeaturesData(const std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>& featuresValue) : features(featuresValue) {}

    static std::shared_ptr<BiomeConsolidatedFeaturesData> create(ReadOnlyBinaryStream& stream) {
		auto features = std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>();
		auto featuresCount = stream.readUnsignedVarInt().value_or(0);
		const bool featuresCountAvailable = stream.canReadCollection(featuresCount);
		for (std::size_t i = 0; i < featuresCount && featuresCountAvailable && !stream.hasOverflowed(); ++i) {
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

} // namespace bedrock::protocol
