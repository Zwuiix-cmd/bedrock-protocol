#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>

class SyncWorldClocksRemoveTimeMarker : public SyncWorldClocksPayload {
public:
    static constexpr int64_t ID = 3;

    uint64_t clockId;
    std::vector<uint64_t> markerIds;

    SyncWorldClocksRemoveTimeMarker() = default;
    virtual ~SyncWorldClocksRemoveTimeMarker() = default;
    explicit SyncWorldClocksRemoveTimeMarker(const uint64_t& clockIdValue, const std::vector<uint64_t>& markerIdsValue) : clockId(clockIdValue), markerIds(markerIdsValue) {}

    static std::shared_ptr<SyncWorldClocksRemoveTimeMarker> create(ReadOnlyBinaryStream& stream) {
		auto clockId = stream.readUnsignedVarLong().value_or(0);
		auto markerIds = std::vector<uint64_t>();
		auto markerIdsCount = stream.readUnsignedVarInt().value_or(0);
		const bool markerIdsCountAvailable = stream.canReadCollection(markerIdsCount);
		for (std::size_t i = 0; i < markerIdsCount && markerIdsCountAvailable && !stream.hasOverflowed(); ++i) {
		    markerIds.push_back(stream.readUnsignedVarLong().value_or(0));
		}
		return std::make_shared<SyncWorldClocksRemoveTimeMarker>(static_cast<int32_t>(clockId), markerIds);
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
		stream.writeUnsignedVarInt(markerIds.size());
		for (const auto& markerId : markerIds) {
			stream.writeUnsignedVarLong(markerId);
		}
    }
};
