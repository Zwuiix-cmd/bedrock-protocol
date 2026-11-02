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
#include <bedrock/protocol/types/GatheringJoinInfo.hpp>
#include <bedrock/protocol/types/PresenceInfo.hpp>
#include <bedrock/protocol/types/StoreEntryPointInfo.hpp>
#include <bedrock/protocol/types/GatheringJoinInfo.hpp>
#include <bedrock/protocol/types/PresenceInfo.hpp>
#include <bedrock/protocol/types/StoreEntryPointInfo.hpp>
#include <bedrock/protocol/types/GatheringJoinInfo.hpp>
#include <bedrock/protocol/types/PresenceInfo.hpp>
#include <bedrock/protocol/types/StoreEntryPointInfo.hpp>

class ServerJoinInformation {
public:
    std::optional<std::shared_ptr<GatheringJoinInfo>> gatheringJoinInfo;
    std::optional<std::shared_ptr<StoreEntryPointInfo>> storeEntryPointInfo;
    std::optional<std::shared_ptr<PresenceInfo>> presenceInfo;

    ServerJoinInformation() = default;
    explicit ServerJoinInformation(
        const std::optional<std::shared_ptr<GatheringJoinInfo>>& gatheringJoinInfoValue,
        const std::optional<std::shared_ptr<StoreEntryPointInfo>>& storeEntryPointInfoValue,
        const std::optional<std::shared_ptr<PresenceInfo>>& presenceInfoValue
    ) :
        gatheringJoinInfo(gatheringJoinInfoValue),
        storeEntryPointInfo(storeEntryPointInfoValue),
        presenceInfo(presenceInfoValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        gatheringJoinInfo = stream.readOptional([&]() {
	return ([&]() {
	auto value = std::make_shared<GatheringJoinInfo>();
	value->read(stream);
	return value;
	}());
});
        storeEntryPointInfo = stream.readOptional([&]() {
	return ([&]() {
	auto value = std::make_shared<StoreEntryPointInfo>();
	value->read(stream);
	return value;
	}());
});
        presenceInfo = stream.readOptional([&]() {
	return ([&]() {
	auto value = std::make_shared<PresenceInfo>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(gatheringJoinInfo, [&](const auto& info) {
			info->write(stream);
		});
		stream.writeOptional(storeEntryPointInfo, [&](const auto& info) {
			info->write(stream);
		});
		stream.writeOptional(presenceInfo, [&](const auto& info) {
			info->write(stream);
		});
    }
};
