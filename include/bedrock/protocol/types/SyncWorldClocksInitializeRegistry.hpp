#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
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

    static std::shared_ptr<SyncWorldClocksInitializeRegistry> create(ReadOnlyBinaryStream& stream) {
		auto clockData = std::vector<std::shared_ptr<SyncWorldClockData>>();
		auto clockDataCount = stream.readUnsignedVarInt().value_or(0);
		const bool clockDataCountAvailable = stream.canReadCollection(clockDataCount);
		for (std::size_t i = 0; i < clockDataCount && clockDataCountAvailable && !stream.hasOverflowed(); ++i) {
		    clockData.push_back(stream.readObject<SyncWorldClockData>());
		}
		return std::make_shared<SyncWorldClocksInitializeRegistry>(clockData);
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
