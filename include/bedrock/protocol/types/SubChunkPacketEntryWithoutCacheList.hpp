#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SubChunkPacketEntryWithoutCache.hpp>
namespace bedrock::protocol {


class SubChunkPacketEntryWithoutCacheList {
public:
    std::vector<std::shared_ptr<SubChunkPacketEntryWithoutCache>> entries;

    SubChunkPacketEntryWithoutCacheList() = default;
    explicit SubChunkPacketEntryWithoutCacheList(const std::vector<std::shared_ptr<SubChunkPacketEntryWithoutCache>>& entriesValue) : entries(entriesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        entries = stream.readObjectList<SubChunkPacketEntryWithoutCache>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(entries, [&](const auto& item) {
            item->write(stream);
        });
    }
};

} // namespace bedrock::protocol
