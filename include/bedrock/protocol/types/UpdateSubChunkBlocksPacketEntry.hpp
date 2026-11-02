#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class UpdateSubChunkBlocksPacketEntry {
public:
    glm::ivec3 blockPosition;
    uint32_t blockRuntimeId;
    uint32_t flags;
    uint64_t syncedUpdateActorUniqueId;
    uint32_t syncedUpdateType;

    UpdateSubChunkBlocksPacketEntry() = default;
    explicit UpdateSubChunkBlocksPacketEntry(
        const glm::ivec3& blockPositionValue,
        const uint32_t& blockRuntimeIdValue,
        const uint32_t& flagsValue,
        const uint64_t& syncedUpdateActorUniqueIdValue,
        const uint32_t& syncedUpdateTypeValue
    ) :
        blockPosition(blockPositionValue),
        blockRuntimeId(blockRuntimeIdValue),
        flags(flagsValue),
        syncedUpdateActorUniqueId(syncedUpdateActorUniqueIdValue),
        syncedUpdateType(syncedUpdateTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        blockPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        flags = stream.readUnsignedVarInt().value_or(0);
        syncedUpdateActorUniqueId = stream.readUnsignedVarLong().value_or(0);
        syncedUpdateType = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeSignedBlockPos(blockPosition);
		stream.writeUnsignedVarInt(blockRuntimeId);
		stream.writeUnsignedVarInt(flags);
		stream.writeUnsignedVarLong(syncedUpdateActorUniqueId);
		stream.writeUnsignedVarInt(syncedUpdateType);
    }
};
