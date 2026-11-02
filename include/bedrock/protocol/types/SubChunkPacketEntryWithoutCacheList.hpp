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
#include <bedrock/protocol/types/SubChunkPacketEntryWithoutCache.hpp>

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
