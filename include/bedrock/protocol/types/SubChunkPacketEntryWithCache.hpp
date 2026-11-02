#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>
namespace bedrock::protocol {


class SubChunkPacketEntryWithCache {
public:
    std::shared_ptr<SubChunkPacketEntryCommon> base;
    uint64_t usedBlobHash;

    SubChunkPacketEntryWithCache() = default;
    explicit SubChunkPacketEntryWithCache(const std::shared_ptr<SubChunkPacketEntryCommon>& baseValue, const uint64_t& usedBlobHashValue) : base(baseValue), usedBlobHash(usedBlobHashValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        base = std::make_shared<SubChunkPacketEntryCommon>();
        base->read(stream);
        usedBlobHash = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		base->write(stream);
		stream.writeUnsignedLong(usedBlobHash);
    }
};

} // namespace bedrock::protocol
