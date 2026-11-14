#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/GatheringJoinInfo.hpp>
#include <bedrock/protocol/types/PresenceInfo.hpp>
#include <bedrock/protocol/types/StoreEntryPointInfo.hpp>
namespace bedrock::protocol {


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
            return stream.readObject<GatheringJoinInfo>();
        });
        storeEntryPointInfo = stream.readOptional([&]() {
            return stream.readObject<StoreEntryPointInfo>();
        });
        presenceInfo = stream.readOptional([&]() {
            return stream.readObject<PresenceInfo>();
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

} // namespace bedrock::protocol
