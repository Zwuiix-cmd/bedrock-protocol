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
#include <bedrock/protocol/types/SyncWorldClockMarkerData.hpp>
#include <bedrock/protocol/types/SyncWorldClockMarkerData.hpp>
#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>

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

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        clockId = stream.readUnsignedVarLong().value_or(0);
        markers = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<SyncWorldClockMarkerData>();
            value->read(stream);
            return value;
            }());
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarLong(clockId);
		stream.writeUnsignedVarInt(markers.size());
		for (const auto& marker : markers) {
			marker->write(stream);
		}
    }
};
