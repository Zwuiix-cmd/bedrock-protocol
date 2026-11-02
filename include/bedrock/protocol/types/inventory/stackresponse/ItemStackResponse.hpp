#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseContainerInfo.hpp>
namespace bedrock::protocol {


class ItemStackResponse {
public:
    enum class Result : uint8_t {
        Ok = 0,
        Error = 1,
    };

    Result result;
    int32_t requestId;
    std::vector<std::shared_ptr<ItemStackResponseContainerInfo>> containerInfos;

    ItemStackResponse() = default;
    explicit ItemStackResponse(
        const Result& resultValue,
        const int32_t& requestIdValue,
        const std::vector<std::shared_ptr<ItemStackResponseContainerInfo>>& containerInfosValue
    ) :
        result(resultValue),
        requestId(requestIdValue),
        containerInfos(containerInfosValue) {
		if ((result != Result::Ok) && (containerInfos.size() != 0)) {
			throw std::exception{};
		}
    }

    static std::shared_ptr<ItemStackResponse> create(ReadOnlyBinaryStream& stream) {
		auto result = stream.readUnsignedByte().value_or(0);
		auto requestId = stream.readVarInt().value_or(0);
		auto containerInfos = std::vector<std::shared_ptr<ItemStackResponseContainerInfo>>();
		if (result == magic_enum::enum_integer(Result::Ok)) {
			auto containerInfosCount = stream.readUnsignedVarInt().value_or(0);
			const bool containerInfosCountAvailable = stream.canReadCollection(containerInfosCount);
			for (std::size_t i = 0; i < containerInfosCount && containerInfosCountAvailable && !stream.hasOverflowed(); ++i) {
			    containerInfos.push_back(stream.readObject<ItemStackResponseContainerInfo>());
			}
		}
		return std::make_shared<ItemStackResponse>(magic_enum::enum_cast<Result>(
	result
).value_or(Result::Ok), requestId, containerInfos);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(result));
		stream.writeVarInt(requestId);
		if (result == Result::Ok) {
			stream.writeUnsignedVarInt(containerInfos.size());
			for (const auto& containerInfo : containerInfos) {
				containerInfo->write(stream);
			}
		}
    }
};

} // namespace bedrock::protocol
