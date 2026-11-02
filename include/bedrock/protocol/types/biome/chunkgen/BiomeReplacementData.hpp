#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>


class BiomeReplacementData {
public:
    int16_t biome;
    int32_t dimension;
    std::vector<int16_t> targetBiomes;
    float amount;
    uint32_t replacementIndex;

    BiomeReplacementData() = default;
    explicit BiomeReplacementData(
        const int16_t& biomeValue,
        const int32_t& dimensionValue,
        const std::vector<int16_t>& targetBiomesValue,
        const float& amountValue,
        const uint32_t& replacementIndexValue
    ) :
        biome(biomeValue),
        dimension(dimensionValue),
        targetBiomes(targetBiomesValue),
        amount(amountValue),
        replacementIndex(replacementIndexValue) {}

    static std::shared_ptr<BiomeReplacementData> create(ReadOnlyBinaryStream& stream) {
		auto biome = stream.readShort().value_or(0);
		auto dimension = stream.readVarInt().value_or(0);
		auto targetBiomes = std::vector<int16_t>();
		auto targetBiomeCount = stream.readUnsignedVarInt().value_or(0);
		for (std::size_t i = 0; (i < targetBiomeCount); ++i) {
			targetBiomes.push_back(stream.readShort().value_or(0));
		}
		auto amount = stream.readFloat().value_or(0.0f);
		auto replacementIndex = stream.readUnsignedInt().value_or(0);
		return std::make_shared<BiomeReplacementData>(biome, dimension, targetBiomes, amount, replacementIndex);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
