#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClockMarkerData.hpp>
#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>
namespace bedrock::protocol {


class SyncWorldClocksAddTimeMarker : public SyncWorldClocksPayload {
public:
    static constexpr int64_t ID = 2;

    uint64_t clockId;
    std::vector<std::shared_ptr<SyncWorldClockMarkerData>> markers;

    SyncWorldClocksAddTimeMarker() = default;
    virtual ~SyncWorldClocksAddTimeMarker() = default;
    explicit SyncWorldClocksAddTimeMarker(
        const uint64_t& clockIdValue,
        const std::vector<std::shared_ptr<SyncWorldClockMarkerData>>& markersValue
    ) :
        clockId(clockIdValue),
        markers(markersValue) {}

    static std::shared_ptr<SyncWorldClocksAddTimeMarker> create(ReadOnlyBinaryStream& stream) {
		auto clockId = stream.readUnsignedVarLong().value_or(0);
		auto markers = std::vector<std::shared_ptr<SyncWorldClockMarkerData>>();
		auto markersCount = stream.readUnsignedVarInt().value_or(0);
		const bool markersCountAvailable = stream.canReadCollection(markersCount);
		for (std::size_t i = 0; i < markersCount && markersCountAvailable && !stream.hasOverflowed(); ++i) {
		    markers.push_back(stream.readObject<SyncWorldClockMarkerData>());
		}
		return std::make_shared<SyncWorldClocksAddTimeMarker>(static_cast<int32_t>(clockId), markers);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarLong(clockId);
		stream.writeUnsignedVarInt(markers.size());
		for (const auto& marker : markers) {
			marker->write(stream);
		}
    }
};

} // namespace bedrock::protocol
