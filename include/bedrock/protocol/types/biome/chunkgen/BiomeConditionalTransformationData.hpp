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
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeWeightedData.hpp>
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
        conditionJSON = stream.readShort().value_or(0);
        minPassingNeighbors = stream.readUnsignedInt().value_or(0);
        weightedBiomes = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<BiomeWeightedData>();
            value->read(stream);
            return value;
            }());
        });
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
