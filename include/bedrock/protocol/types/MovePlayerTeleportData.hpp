#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class MovePlayerTeleportData {
public:
    uint32_t cause;
    uint32_t sourceActorType;

    MovePlayerTeleportData() = default;
    explicit MovePlayerTeleportData(const uint32_t& causeValue, const uint32_t& sourceActorTypeValue) : cause(causeValue), sourceActorType(sourceActorTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        cause = stream.readUnsignedInt().value_or(0);
        sourceActorType = stream.readUnsignedInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(cause);
		stream.writeUnsignedInt(sourceActorType);
    }
};

} // namespace bedrock::protocol
