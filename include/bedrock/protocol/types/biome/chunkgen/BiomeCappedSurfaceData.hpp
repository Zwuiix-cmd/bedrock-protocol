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

class BiomeCappedSurfaceData {
public:
    std::vector<int32_t> floorBlocks;
    std::vector<int32_t> ceilingBlocks;
    std::optional<int32_t> seaBlock;
    std::optional<int32_t> foundationBlock;
    std::optional<int32_t> beachBlock;

    BiomeCappedSurfaceData() = default;
    explicit BiomeCappedSurfaceData(
        const std::vector<int32_t>& floorBlocksValue,
        const std::vector<int32_t>& ceilingBlocksValue,
        const std::optional<int32_t>& seaBlockValue,
        const std::optional<int32_t>& foundationBlockValue,
        const std::optional<int32_t>& beachBlockValue
    ) :
        floorBlocks(floorBlocksValue),
        ceilingBlocks(ceilingBlocksValue),
        seaBlock(seaBlockValue),
        foundationBlock(foundationBlockValue),
        beachBlock(beachBlockValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        floorBlocks = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
        ceilingBlocks = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
        seaBlock = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
        foundationBlock = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
        beachBlock = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
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
