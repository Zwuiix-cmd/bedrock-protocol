#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClockMarkerData.hpp>

class SyncWorldClockData {
public:
    uint64_t id;
    std::string name;
    int32_t time;
    bool paused;
    std::vector<std::shared_ptr<SyncWorldClockMarkerData>> markers;

    SyncWorldClockData() = default;
    explicit SyncWorldClockData(
        const uint64_t& idValue,
        const std::string& nameValue,
        const int32_t& timeValue,
        const bool& pausedValue,
        const std::vector<std::shared_ptr<SyncWorldClockMarkerData>>& markersValue
    ) :
        id(idValue),
        name(nameValue),
        time(timeValue),
        paused(pausedValue),
        markers(markersValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        id = stream.readUnsignedVarLong().value_or(0);
        name = stream.readString().value_or("");
        time = stream.readVarInt().value_or(0);
        paused = stream.readBool().value_or(false);
        markers = stream.readObjectList<SyncWorldClockMarkerData>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarLong(id);
		stream.writeString(name);
		stream.writeVarInt(time);
		stream.writeBool(paused);
		stream.writeUnsignedVarInt(markers.size());
		for (const auto& marker : markers) {
			marker->write(stream);
		}
    }
};
