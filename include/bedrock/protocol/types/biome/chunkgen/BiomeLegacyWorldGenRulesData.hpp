#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <stdexcept>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeConditionalTransformationData.hpp>

class BiomeLegacyWorldGenRulesData {
public:
    std::vector<std::shared_ptr<BiomeConditionalTransformationData>> legacyPreHills;

    BiomeLegacyWorldGenRulesData() = default;
    explicit BiomeLegacyWorldGenRulesData(const std::vector<std::shared_ptr<BiomeConditionalTransformationData>>& legacyPreHillsValue) : legacyPreHills(legacyPreHillsValue) {}

    static std::shared_ptr<BiomeLegacyWorldGenRulesData> create(ReadOnlyBinaryStream& stream) {
		auto legacyPreHills = std::vector<std::shared_ptr<BiomeConditionalTransformationData>>();
		auto legacyPreHillsCount = stream.readUnsignedVarInt().value_or(0);
		if (legacyPreHillsCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < legacyPreHillsCount; ++i) {
		    legacyPreHills.push_back(stream.readObject<BiomeConditionalTransformationData>());
		}
		return std::make_shared<BiomeLegacyWorldGenRulesData>(legacyPreHills);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(legacyPreHills.size());
		for (const auto& biome : legacyPreHills) {
			biome->write(stream);
		}
    }
};
