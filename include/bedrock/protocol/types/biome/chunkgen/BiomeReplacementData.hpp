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

class BiomeReplacementData {
public:
    int16_t biome;
    int32_t dimension;
    std::vector<int32_t> targetBiomes;
    float amount;
    uint32_t replacementIndex;

    BiomeReplacementData() = default;
    explicit BiomeReplacementData(
        const int16_t& biomeValue,
        const int32_t& dimensionValue,
        const std::vector<int32_t>& targetBiomesValue,
        const float& amountValue,
        const uint32_t& replacementIndexValue
    ) :
        biome(biomeValue),
        dimension(dimensionValue),
        targetBiomes(targetBiomesValue),
        amount(amountValue),
        replacementIndex(replacementIndexValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        biome = stream.readShort().value_or(0);
        dimension = stream.readVarInt().value_or(0);
        amount = stream.readFloat().value_or(0.0f);
        replacementIndex = stream.readUnsignedInt().value_or(0);
        targetBiomes = stream.readList([&]() {
	return stream.readInt().value_or(0);
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeShort(biome);
		stream.writeVarInt(dimension);
		stream.writeUnsignedVarInt(targetBiomes.size());
		for (const auto& biomeValue : targetBiomes) {
			stream.writeShort(biomeValue);
		}
		stream.writeFloat(amount);
		stream.writeUnsignedInt(replacementIndex);
    }
};
