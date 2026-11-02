#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClockMarkerData.hpp>
namespace bedrock::protocol {


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

    static std::shared_ptr<SyncWorldClockData> create(ReadOnlyBinaryStream& stream) {
		auto id = stream.readUnsignedVarLong().value_or(0);
		auto name = stream.readString().value_or("");
		auto time = stream.readVarInt().value_or(0);
		auto paused = stream.readBool().value_or(false);
		auto markers = std::vector<std::shared_ptr<SyncWorldClockMarkerData>>();
		auto markersCount = stream.readUnsignedVarInt().value_or(0);
		const bool markersCountAvailable = stream.canReadCollection(markersCount);
		for (std::size_t i = 0; i < markersCount && markersCountAvailable && !stream.hasOverflowed(); ++i) {
		    markers.push_back(stream.readObject<SyncWorldClockMarkerData>());
		}
		return std::make_shared<SyncWorldClockData>(static_cast<int32_t>(id), name, time, paused, markers);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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

} // namespace bedrock::protocol
