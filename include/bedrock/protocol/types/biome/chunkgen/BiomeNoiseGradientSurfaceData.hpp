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
#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.hpp>
#include <bedrock/protocol/types/biome/chunkgen/BiomeNoiseBlockSpecifier.hpp>

class BiomeNoiseGradientSurfaceData {
public:
    std::vector<int32_t> nonReplaceableBlocks;
    std::vector<std::shared_ptr<BiomeNoiseBlockSpecifier>> gradientBlocks;
    std::string noiseSeed;
    uint32_t firstOctave;
    std::vector<float> amplitudes;

    BiomeNoiseGradientSurfaceData() = default;
    explicit BiomeNoiseGradientSurfaceData(
        const std::vector<int32_t>& nonReplaceableBlocksValue,
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

    virtual void read(ReadOnlyBinaryStream& stream) {
        noiseSeed = stream.readString().value_or("");
        firstOctave = stream.readUnsignedInt().value_or(0);
        nonReplaceableBlocks = stream.readList([&]() {
			return stream.readInt().value_or(0);
		});
        gradientBlocks = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<BiomeNoiseBlockSpecifier>();
			value->read(stream);
			return value;
			}());
		});
        amplitudes = stream.readList([&]() {
			return stream.readFloat().value_or(0.0f);
		});
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
