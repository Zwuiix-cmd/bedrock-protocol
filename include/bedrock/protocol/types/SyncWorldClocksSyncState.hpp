#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClockStateData.hpp>
#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>

class SyncWorldClocksSyncState : public SyncWorldClocksPayload {
public:
    static constexpr int64_t ID = 0;

    std::vector<std::shared_ptr<SyncWorldClockStateData>> clockData;

    SyncWorldClocksSyncState() = default;
    virtual ~SyncWorldClocksSyncState() = default;
    explicit SyncWorldClocksSyncState(const std::vector<std::shared_ptr<SyncWorldClockStateData>>& clockDataValue) : clockData(clockDataValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        clockData = stream.readObjectList<SyncWorldClockStateData>();
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(clockData.size());
		for (const auto& data : clockData) {
			data->write(stream);
		}
    }
};
