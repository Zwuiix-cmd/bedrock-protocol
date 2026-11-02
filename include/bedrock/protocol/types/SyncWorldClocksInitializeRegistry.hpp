#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/SyncWorldClockData.hpp>
#include <bedrock/protocol/types/SyncWorldClocksPayload.hpp>

class SyncWorldClocksInitializeRegistry : public SyncWorldClocksPayload {
public:
    static constexpr int64_t ID = 1;

    std::vector<std::shared_ptr<SyncWorldClockData>> clockData;

    SyncWorldClocksInitializeRegistry() = default;
    virtual ~SyncWorldClocksInitializeRegistry() = default;
    explicit SyncWorldClocksInitializeRegistry(const std::vector<std::shared_ptr<SyncWorldClockData>>& clockDataValue) : clockData(clockDataValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        clockData = stream.readObjectList<SyncWorldClockData>();
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(clockData.size());
		for (const auto& data : clockData) {
			data->write(stream);
		}
    }
};
