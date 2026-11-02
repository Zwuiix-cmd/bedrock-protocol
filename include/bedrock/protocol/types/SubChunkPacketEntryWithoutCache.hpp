#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>

#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>

class SubChunkPacketEntryWithoutCache {
public:
    std::shared_ptr<SubChunkPacketEntryCommon> base;

    SubChunkPacketEntryWithoutCache() = default;
    explicit SubChunkPacketEntryWithoutCache(const std::shared_ptr<SubChunkPacketEntryCommon>& baseValue) : base(baseValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        base = std::make_shared<SubChunkPacketEntryCommon>();
        base->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		base->write(stream);
    }
};
