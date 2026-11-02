#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class SyncWorldClockStateData {
public:
    uint64_t clockId;
    int32_t time;
    bool paused;

    SyncWorldClockStateData() = default;
    explicit SyncWorldClockStateData(const uint64_t& clockIdValue, const int32_t& timeValue, const bool& pausedValue) : clockId(clockIdValue), time(timeValue), paused(pausedValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        clockId = stream.readUnsignedVarLong().value_or(0);
        time = stream.readVarInt().value_or(0);
        paused = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarLong(clockId);
		stream.writeVarInt(time);
		stream.writeBool(paused);
    }
};
