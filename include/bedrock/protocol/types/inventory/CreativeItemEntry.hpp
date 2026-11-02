#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>

class CreativeItemEntry {
public:
    int32_t entryId;
    NetworkItemDescriptor item;
    uint32_t groupId;

    CreativeItemEntry() = default;
    explicit CreativeItemEntry(const int32_t& entryIdValue, const NetworkItemDescriptor& itemValue, const uint32_t& groupIdValue) : entryId(entryIdValue), item(itemValue), groupId(groupIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        groupId = stream.readUnsignedVarInt().value_or(0);
        entryId = stream.readInt().value_or(0);
        item = stream.readValue<NetworkItemDescriptor>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(groupId);
        stream.writeInt(entryId);
        item.write(stream);
    }
};
