#pragma once

#include <cstdint>
#include <string>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

namespace bedrock::protocol {


struct NetworkItemDescriptor {
    int32_t id = 0;
    uint32_t meta = 0;
    uint16_t count = 0;
    int32_t blockRuntimeId = 0;
    std::string rawExtraData = "";

    void write(BinaryStream& stream) const {
        stream.writeVarInt(id);
        stream.writeUnsignedShort(count);
        stream.writeUnsignedVarInt(meta);
        stream.writeVarInt(blockRuntimeId);
        stream.writeString(rawExtraData);
    }

    void read(ReadOnlyBinaryStream& stream) {
        id = stream.readVarInt().value_or(0);
        count = stream.readUnsignedShort().value_or(0);
        meta = stream.readUnsignedVarInt().value_or(0);
        blockRuntimeId = stream.readVarInt().value_or(0);
        if (!stream.readString(rawExtraData)) {
            rawExtraData.clear();
        }
    }
};


} // namespace bedrock::protocol
