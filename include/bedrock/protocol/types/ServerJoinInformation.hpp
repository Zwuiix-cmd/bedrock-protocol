#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/GatheringJoinInfo.hpp>
namespace bedrock::protocol {


class ServerJoinInformation {
public:
    std::optional<std::shared_ptr<GatheringJoinInfo>> gatheringJoinInfo;

    ServerJoinInformation() = default;
    explicit ServerJoinInformation(const std::optional<std::shared_ptr<GatheringJoinInfo>>& gatheringJoinInfoValue) : gatheringJoinInfo(gatheringJoinInfoValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        gatheringJoinInfo = stream.readOptional([&]() {
            return stream.readObject<GatheringJoinInfo>();
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(gatheringJoinInfo, [&](const auto& info) {
			info->write(stream);
		});
    }
};

} // namespace bedrock::protocol
