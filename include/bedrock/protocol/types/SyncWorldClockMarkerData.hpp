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
