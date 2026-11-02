#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class BiomeSurfaceMaterialData {
public:
    uint32_t topBlock;
    uint32_t midBlock;
    uint32_t seaFloorBlock;
    uint32_t foundationBlock;
    uint32_t seaBlock;
    int32_t seaFloorDepth;

    BiomeSurfaceMaterialData() = default;
    explicit BiomeSurfaceMaterialData(
        const uint32_t& topBlockValue,
        const uint32_t& midBlockValue,
        const uint32_t& seaFloorBlockValue,
        const uint32_t& foundationBlockValue,
        const uint32_t& seaBlockValue,
        const int32_t& seaFloorDepthValue
    ) :
        topBlock(topBlockValue),
        midBlock(midBlockValue),
        seaFloorBlock(seaFloorBlockValue),
        foundationBlock(foundationBlockValue),
        seaBlock(seaBlockValue),
        seaFloorDepth(seaFloorDepthValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        topBlock = stream.readUnsignedInt().value_or(0);
        midBlock = stream.readUnsignedInt().value_or(0);
        seaFloorBlock = stream.readUnsignedInt().value_or(0);
        foundationBlock = stream.readUnsignedInt().value_or(0);
        seaBlock = stream.readUnsignedInt().value_or(0);
        seaFloorDepth = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(topBlock);
		stream.writeUnsignedInt(midBlock);
		stream.writeUnsignedInt(seaFloorBlock);
		stream.writeUnsignedInt(foundationBlock);
		stream.writeUnsignedInt(seaBlock);
		stream.writeInt(seaFloorDepth);
    }
};

} // namespace bedrock::protocol
