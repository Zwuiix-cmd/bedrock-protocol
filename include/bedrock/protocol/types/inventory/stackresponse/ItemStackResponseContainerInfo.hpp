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
#include <bedrock/protocol/types/inventory/FullContainerName.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/FullContainerName.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.hpp>
#include <bedrock/protocol/types/inventory/FullContainerName.hpp>
#include <bedrock/protocol/types/inventory/stackresponse/ItemStackResponseSlotInfo.hpp>

class ItemStackResponseContainerInfo {
public:
    std::shared_ptr<FullContainerName> containerName;
    std::vector<std::shared_ptr<ItemStackResponseSlotInfo>> slots;

    ItemStackResponseContainerInfo() = default;
    explicit ItemStackResponseContainerInfo(
        const std::shared_ptr<FullContainerName>& containerNameValue,
        const std::vector<std::shared_ptr<ItemStackResponseSlotInfo>>& slotsValue
    ) :
        containerName(containerNameValue),
        slots(slotsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerName = ([&]() {
    auto value = std::make_shared<FullContainerName>();
    value->read(stream);
    return value;
}());
        slots = stream.readList([&]() {
	return ([&]() {
	auto value = std::make_shared<ItemStackResponseSlotInfo>();
	value->read(stream);
	return value;
	}());
});
    }

    virtual void write(BinaryStream& stream) const {
		containerName->write(stream);
		stream.writeUnsignedVarInt(slots.size());
		for (const auto& slot : slots) {
			slot->write(stream);
		}
    }
};
