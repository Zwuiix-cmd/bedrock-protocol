#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>
#include <string>


class SyncWorldClockMarkerData {
public:
    uint64_t id;
    std::string name;
    int32_t time;
    std::optional<int32_t> period;

    SyncWorldClockMarkerData() = default;
    explicit SyncWorldClockMarkerData(
        const uint64_t& idValue,
        const std::string& nameValue,
        const int32_t& timeValue,
        const std::optional<int32_t>& periodValue
    ) :
        id(idValue),
        name(nameValue),
        time(timeValue),
        period(periodValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readUnsignedVarLong().value_or(0);
        name = stream.readString().value_or("");
        time = stream.readVarInt().value_or(0);
        period = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarLong(id);
		stream.writeString(name);
		stream.writeVarInt(time);
		stream.writeOptional(period, [&](const auto& value) {
			stream.writeInt(value);
		});
    }
};
