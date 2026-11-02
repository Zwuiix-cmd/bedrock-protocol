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
