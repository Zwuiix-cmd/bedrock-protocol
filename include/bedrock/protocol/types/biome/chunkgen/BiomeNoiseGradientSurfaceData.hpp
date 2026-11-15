#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.hpp>
namespace bedrock::protocol {


class BiomeNoiseGradientSurfaceData {
public:
    std::vector<uint32_t> nonReplaceableBlocks;
    std::vector<std::shared_ptr<BiomeNoiseBlockSpecifier>> gradientBlocks;
    std::string noiseSeed;
    uint32_t firstOctave;
    std::vector<float> amplitudes;

    BiomeNoiseGradientSurfaceData() = default;
    explicit BiomeNoiseGradientSurfaceData(
        const std::vector<uint32_t>& nonReplaceableBlocksValue,
        const std::vector<std::shared_ptr<BiomeNoiseBlockSpecifier>>& gradientBlocksValue,
        const std::string& noiseSeedValue,
        const uint32_t& firstOctaveValue,
        const std::vector<float>& amplitudesValue
    ) :
        nonReplaceableBlocks(nonReplaceableBlocksValue),
        gradientBlocks(gradientBlocksValue),
        noiseSeed(noiseSeedValue),
        firstOctave(firstOctaveValue),
        amplitudes(amplitudesValue) {}

    static std::shared_ptr<BiomeNoiseGradientSurfaceData> create(ReadOnlyBinaryStream& stream) {
		auto nonReplaceableBlocks = std::vector<uint32_t>();
		auto nonReplaceableBlocksCount = stream.readUnsignedVarInt().value_or(0);
		const bool nonReplaceableBlocksCountAvailable = stream.canReadCollection(nonReplaceableBlocksCount);
		for (std::size_t i = 0; i < nonReplaceableBlocksCount && nonReplaceableBlocksCountAvailable && !stream.hasOverflowed(); ++i) {
		    nonReplaceableBlocks.push_back(stream.readUnsignedInt().value_or(0));
		}
		auto gradientBlocks = std::vector<std::shared_ptr<BiomeNoiseBlockSpecifier>>();
		auto gradientBlocksCount = stream.readUnsignedVarInt().value_or(0);
		const bool gradientBlocksCountAvailable = stream.canReadCollection(gradientBlocksCount);
		for (std::size_t i = 0; i < gradientBlocksCount && gradientBlocksCountAvailable && !stream.hasOverflowed(); ++i) {
		    gradientBlocks.push_back(stream.readObject<BiomeNoiseBlockSpecifier>());
		}
		auto noiseSeed = stream.readString().value_or("");
		auto firstOctave = stream.readUnsignedInt().value_or(0);
		auto amplitudes = std::vector<float>();
		auto amplitudesCount = stream.readUnsignedVarInt().value_or(0);
		const bool amplitudesCountAvailable = stream.canReadCollection(amplitudesCount);
		for (std::size_t i = 0; i < amplitudesCount && amplitudesCountAvailable && !stream.hasOverflowed(); ++i) {
		    amplitudes.push_back(stream.readFloat().value_or(0.0f));
		}
		return std::make_shared<BiomeNoiseGradientSurfaceData>(nonReplaceableBlocks, gradientBlocks, noiseSeed, static_cast<int32_t>(firstOctave), amplitudes);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(nonReplaceableBlocks.size());
		for (const auto& value : nonReplaceableBlocks) {
			stream.writeUnsignedInt(value);
		}
		stream.writeUnsignedVarInt(gradientBlocks.size());
		for (const auto& value : gradientBlocks) {
			value->write(stream);
		}
		stream.writeString(noiseSeed);
		stream.writeUnsignedInt(firstOctave);
		stream.writeUnsignedVarInt(amplitudes.size());
		for (const auto& value : amplitudes) {
			stream.writeFloat(value);
		}
    }
};

} // namespace bedrock::protocol
