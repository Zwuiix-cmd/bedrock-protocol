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

class ServerJoinInformation {
public:
    std::optional<std::shared_ptr<GatheringJoinInfo>> gatheringJoinInfo;

    ServerJoinInformation() = default;
    explicit ServerJoinInformation(const std::optional<std::shared_ptr<GatheringJoinInfo>>& gatheringJoinInfoValue) : gatheringJoinInfo(gatheringJoinInfoValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        gatheringJoinInfo = stream.readOptional([&]() {
            auto value = std::make_shared<GatheringJoinInfo>();
            value->read(stream);
            return value;
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(gatheringJoinInfo, [&](const auto& info) {
			info->write(stream);
		});
    }
};
