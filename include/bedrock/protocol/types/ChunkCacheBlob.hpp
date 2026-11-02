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

class ChunkCacheBlob {
public:
    uint64_t hash;
    std::string payload;

    ChunkCacheBlob() = default;
    explicit ChunkCacheBlob(const uint64_t& hashValue, const std::string& payloadValue) : hash(hashValue), payload(payloadValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        hash = stream.readUnsignedLong().value_or(0);
        payload = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedLong(hash);
        stream.writeString(payload);
    }
};
