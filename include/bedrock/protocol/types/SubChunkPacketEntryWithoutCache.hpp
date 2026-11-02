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
#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>
#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>
#include <bedrock/protocol/types/SubChunkPacketEntryCommon.hpp>

class SubChunkPacketEntryWithoutCache {
public:
    std::shared_ptr<SubChunkPacketEntryCommon> base;

    SubChunkPacketEntryWithoutCache() = default;
    explicit SubChunkPacketEntryWithoutCache(const std::shared_ptr<SubChunkPacketEntryCommon>& baseValue) : base(baseValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        base = ([&]() {
    auto value = std::make_shared<SubChunkPacketEntryCommon>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
		base->write(stream);
    }
};
