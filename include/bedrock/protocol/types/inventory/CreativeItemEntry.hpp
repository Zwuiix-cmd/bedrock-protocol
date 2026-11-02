#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
namespace bedrock::protocol {


class CreativeItemEntry {
public:
    uint32_t entryId;
    NetworkItemDescriptor item;
    uint32_t groupId;

    CreativeItemEntry() = default;
    explicit CreativeItemEntry(const uint32_t& entryIdValue, const NetworkItemDescriptor& itemValue, const uint32_t& groupIdValue) : entryId(entryIdValue), item(itemValue), groupId(groupIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        entryId = stream.readUnsignedVarInt().value_or(0);
        item = stream.readValue<NetworkItemDescriptor>();
        groupId = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(entryId);
        item.write(stream);
        stream.writeUnsignedVarInt(groupId);
    }
};

} // namespace bedrock::protocol
