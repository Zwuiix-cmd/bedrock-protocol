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
