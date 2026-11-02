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

class ItemStackResponse {
public:
    enum class Result : uint8_t {
        Ok = 0,
        Error = 1,
    };

    Result result;
    int32_t requestId;
    std::optional<std::vector<std::shared_ptr<ItemStackResponseContainerInfo>>> containerInfos;

    ItemStackResponse() = default;
    explicit ItemStackResponse(
        const Result& resultValue,
        const int32_t& requestIdValue,
        const std::optional<std::vector<std::shared_ptr<ItemStackResponseContainerInfo>>>& containerInfosValue
    ) :
        result(resultValue),
        requestId(requestIdValue),
        containerInfos(containerInfosValue) {
		if (((magic_enum::enum_integer(result) != magic_enum::enum_integer(Result::Ok)) && containerInfos.has_value())) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        result = magic_enum::enum_cast<Result>(stream.readUnsignedByte().value_or(0)).value_or(Result::Ok);
        requestId = stream.readInt().value_or(0);
        containerInfos = stream.readOptional([&]() {
	return stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<ItemStackResponseContainerInfo>();
	value->read(stream);
	return value;
	}());
	});
});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(result));
		stream.writeInt(requestId);
		stream.writeOptional(containerInfos, [&](const auto& list) {
			stream.writeList(list, [&](const auto& v) {
			v->write(stream);
			});
		});
    }
};
