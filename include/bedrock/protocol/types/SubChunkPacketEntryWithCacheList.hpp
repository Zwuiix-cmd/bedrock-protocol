#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SubChunkPacketEntryWithCache.hpp>

class SubChunkPacketEntryWithCacheList {
public:
    std::vector<std::shared_ptr<SubChunkPacketEntryWithCache>> entries;

    SubChunkPacketEntryWithCacheList() = default;
    explicit SubChunkPacketEntryWithCacheList(const std::vector<std::shared_ptr<SubChunkPacketEntryWithCache>>& entriesValue) : entries(entriesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        entries = stream.readObjectList<SubChunkPacketEntryWithCache>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(entries, [&](const auto& item) {
            item->write(stream);
        });
    }
};
