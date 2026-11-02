#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


struct NetworkItemStack {
    int32_t stackId = 0;
    NetworkItemDescriptor descriptor;

    void write(BinaryStream& stream) const {
        stream.writeShort(static_cast<int16_t>(descriptor.id));
        stream.writeUnsignedShort(descriptor.count);
        stream.writeUnsignedVarInt(descriptor.meta);
        const bool hasId = stackId != 0;
        stream.writeBool(hasId);
        if (hasId) {
            stream.writeVarInt(stackId);
        }
        stream.writeUnsignedVarInt(static_cast<uint32_t>(descriptor.blockRuntimeId));
        stream.writeString(descriptor.rawExtraData);
    }

    void read(ReadOnlyBinaryStream& stream) {
        descriptor = NetworkItemDescriptor();
        descriptor.id = stream.readShort().value_or(0);
        descriptor.count = stream.readUnsignedShort().value_or(0);
        descriptor.meta = stream.readUnsignedVarInt().value_or(0);
        stackId = stream.readBool().value_or(false) ? stream.readVarInt().value_or(0) : 0;
        descriptor.blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        if (!stream.readString(descriptor.rawExtraData)) {
            descriptor.rawExtraData.clear();
        }
    }

};


} // namespace bedrock::protocol
