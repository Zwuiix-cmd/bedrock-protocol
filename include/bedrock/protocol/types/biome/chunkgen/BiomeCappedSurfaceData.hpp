#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>
#include <vector>


class BiomeCappedSurfaceData {
public:
    std::vector<uint32_t> floorBlocks;
    std::vector<uint32_t> ceilingBlocks;
    std::optional<uint32_t> seaBlock;
    std::optional<uint32_t> foundationBlock;
    std::optional<uint32_t> beachBlock;

    BiomeCappedSurfaceData() = default;
    explicit BiomeCappedSurfaceData(
        const std::vector<uint32_t>& floorBlocksValue,
        const std::vector<uint32_t>& ceilingBlocksValue,
        const std::optional<uint32_t>& seaBlockValue,
        const std::optional<uint32_t>& foundationBlockValue,
        const std::optional<uint32_t>& beachBlockValue
    ) :
        floorBlocks(floorBlocksValue),
        ceilingBlocks(ceilingBlocksValue),
        seaBlock(seaBlockValue),
        foundationBlock(foundationBlockValue),
        beachBlock(beachBlockValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        floorBlocks = stream.readList([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        ceilingBlocks = stream.readList([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        seaBlock = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        foundationBlock = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        beachBlock = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(floorBlocks.size());
		for (const auto& block : floorBlocks) {
			stream.writeUnsignedInt(block);
		}
		stream.writeUnsignedVarInt(ceilingBlocks.size());
		for (const auto& block : ceilingBlocks) {
			stream.writeUnsignedInt(block);
		}
		stream.writeOptional(seaBlock, [&](const auto& value) {
			stream.writeUnsignedInt(value);
		});
		stream.writeOptional(foundationBlock, [&](const auto& value) {
			stream.writeUnsignedInt(value);
		});
		stream.writeOptional(beachBlock, [&](const auto& value) {
			stream.writeUnsignedInt(value);
		});
    }
};
