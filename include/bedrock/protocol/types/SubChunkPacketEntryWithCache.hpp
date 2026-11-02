#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>

class SubChunkPacketEntryWithCache {
public:
    std::shared_ptr<SubChunkPacketEntryCommon> base;
    uint64_t usedBlobHash;

    SubChunkPacketEntryWithCache() = default;
    explicit SubChunkPacketEntryWithCache(const std::shared_ptr<SubChunkPacketEntryCommon>& baseValue, const uint64_t& usedBlobHashValue) : base(baseValue), usedBlobHash(usedBlobHashValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        usedBlobHash = stream.readUnsignedLong().value_or(0);
        base = std::make_shared<SubChunkPacketEntryCommon>();
        base->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		base->write(stream);
		stream.writeUnsignedLong(usedBlobHash);
    }
};
