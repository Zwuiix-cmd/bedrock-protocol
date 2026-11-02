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
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConsolidatedFeatureData.hpp>

class BiomeConsolidatedFeaturesData {
public:
    std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>> features;

    BiomeConsolidatedFeaturesData() = default;
    explicit BiomeConsolidatedFeaturesData(const std::vector<std::shared_ptr<BiomeConsolidatedFeatureData>>& featuresValue) : features(featuresValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        features = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeConsolidatedFeatureData>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(features.size());
		for (const auto& feature : features) {
			feature->write(stream);
		}
    }
};
