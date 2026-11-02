#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>

class SyncWorldClocksRemoveTimeMarker : public SyncWorldClocksPayload {
public:
    static constexpr int64_t ID = 3;

    uint64_t clockId;
    std::vector<int32_t> markerIds;

    SyncWorldClocksRemoveTimeMarker() = default;
    virtual ~SyncWorldClocksRemoveTimeMarker() = default;
    explicit SyncWorldClocksRemoveTimeMarker(const uint64_t& clockIdValue, const std::vector<int32_t>& markerIdsValue) : clockId(clockIdValue), markerIds(markerIdsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        clockId = stream.readUnsignedVarLong().value_or(0);
        markerIds = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarLong(clockId);
		stream.writeUnsignedVarInt(markerIds.size());
		for (const auto& markerId : markerIds) {
			stream.writeUnsignedVarLong(markerId);
		}
    }
};
