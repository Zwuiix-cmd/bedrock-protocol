#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<SyncWorldClocksSyncState> create(ReadOnlyBinaryStream& stream) {
		auto clockData = std::vector<std::shared_ptr<SyncWorldClockStateData>>();
		auto clockDataCount = stream.readUnsignedVarInt().value_or(0);
		if (clockDataCount > ReadOnlyBinaryStream::MaxCollectionSize) {
		    throw std::runtime_error("protocol collection is too large");
		}
		for (std::size_t i = 0; i < clockDataCount; ++i) {
		    clockData.push_back(stream.readObject<SyncWorldClockStateData>());
		}
		return std::make_shared<SyncWorldClocksSyncState>(clockData);
    }

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(clockData.size());
		for (const auto& data : clockData) {
			data->write(stream);
		}
    }
};
