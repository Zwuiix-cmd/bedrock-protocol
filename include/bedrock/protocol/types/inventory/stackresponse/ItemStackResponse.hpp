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
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.hpp>

class ItemStackResponse {
public:
    static constexpr int64_t RESULT_OK = 0;
    static constexpr int64_t RESULT_ERROR = 1;

    uint8_t result;
    int32_t requestId;
    std::vector<std::shared_ptr<ItemStackResponseContainerInfo>> containerInfos;

    ItemStackResponse() = default;
    explicit ItemStackResponse(
        const uint8_t& resultValue,
        const int32_t& requestIdValue,
        const std::vector<std::shared_ptr<ItemStackResponseContainerInfo>>& containerInfosValue
    ) :
        result(resultValue),
        requestId(requestIdValue),
        containerInfos(containerInfosValue) {
		if (((result != RESULT_OK) && (containerInfos.size() != 0))) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        result = stream.readUnsignedByte().value_or(0);
        requestId = stream.readInt().value_or(0);
        containerInfos = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<ItemStackResponseContainerInfo>();
            value->read(stream);
            return value;
            }());
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(result);
		stream.writeInt(requestId);
		if ((result == RESULT_OK)) {
			stream.writeUnsignedVarInt(containerInfos.size());
			for (const auto& containerInfo : containerInfos) {
				containerInfo->write(stream);
			}
		}
    }
};
