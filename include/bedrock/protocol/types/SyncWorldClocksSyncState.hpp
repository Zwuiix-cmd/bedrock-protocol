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
#include <bedrock/protocol/types/SyncWorldClockStateData.hpp>
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
        clockData = stream.readList([&]() {
			return ([&]() {
			auto value = std::make_shared<SyncWorldClockStateData>();
			value->read(stream);
			return value;
			}());
		});
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(clockData.size());
		for (const auto& data : clockData) {
			data->write(stream);
		}
    }
};
