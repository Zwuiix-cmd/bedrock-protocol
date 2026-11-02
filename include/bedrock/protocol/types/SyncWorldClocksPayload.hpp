#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>


class SyncWorldClocksPayload {
public:

    SyncWorldClocksPayload() = default;
    virtual ~SyncWorldClocksPayload() = default;

    virtual int32_t getTypeId() const = 0;
    virtual void write(BinaryStream& stream) const = 0;
    virtual void read(ReadOnlyBinaryStream& stream) = 0;

    static std::shared_ptr<SyncWorldClocksPayload> create(ReadOnlyBinaryStream& stream);
};
