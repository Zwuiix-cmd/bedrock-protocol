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

class BiomeLegacyWorldGenRulesData {
public:
    std::vector<std::shared_ptr<BiomeConditionalTransformationData>> legacyPreHills;

    BiomeLegacyWorldGenRulesData() = default;
    explicit BiomeLegacyWorldGenRulesData(const std::vector<std::shared_ptr<BiomeConditionalTransformationData>>& legacyPreHillsValue) : legacyPreHills(legacyPreHillsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        legacyPreHills = stream.readObjectList<BiomeConditionalTransformationData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(legacyPreHills.size());
		for (const auto& biome : legacyPreHills) {
			biome->write(stream);
		}
    }
};
