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
#include <bedrock/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedTemperatureData.hpp>

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

    virtual void read(ReadOnlyBinaryStream& stream) {
        hillTransformations = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeWeightedData>();
	value->read(stream);
	return value;
	}());
});
        mutateTransformations = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeWeightedData>();
	value->read(stream);
	return value;
	}());
});
        riverTransformations = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeWeightedData>();
	value->read(stream);
	return value;
	}());
});
        shoreTransformations = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeWeightedData>();
	value->read(stream);
	return value;
	}());
});
        preHillsEdges = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeConditionalTransformationData>();
	value->read(stream);
	return value;
	}());
});
        postShoreEdges = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeConditionalTransformationData>();
	value->read(stream);
	return value;
	}());
});
        climates = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<BiomeWeightedTemperatureData>();
	value->read(stream);
	return value;
	}());
});
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
