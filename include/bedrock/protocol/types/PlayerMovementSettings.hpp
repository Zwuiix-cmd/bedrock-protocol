#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class PlayerMovementSettings {
public:
    int32_t rewindHistorySize;
    bool serverAuthoritativeBlockBreaking;

    PlayerMovementSettings() = default;
    explicit PlayerMovementSettings(
        const int32_t& rewindHistorySizeValue,
        const bool& serverAuthoritativeBlockBreakingValue
    ) :
        rewindHistorySize(rewindHistorySizeValue),
        serverAuthoritativeBlockBreaking(serverAuthoritativeBlockBreakingValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        rewindHistorySize = stream.readVarInt().value_or(0);
        serverAuthoritativeBlockBreaking = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(rewindHistorySize);
		stream.writeBool(serverAuthoritativeBlockBreaking);
    }
};

} // namespace bedrock::protocol
