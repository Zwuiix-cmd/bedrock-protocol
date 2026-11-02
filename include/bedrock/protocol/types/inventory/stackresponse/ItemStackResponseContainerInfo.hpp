#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <memory>
#include <vector>

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

    static std::shared_ptr<ItemStackResponseContainerInfo> create(ReadOnlyBinaryStream& stream) {
		auto containerName = std::make_shared<FullContainerName>();
		containerName->read(stream);
		auto slots = std::vector<std::shared_ptr<ItemStackResponseSlotInfo>>();
		auto slotsCount = stream.readUnsignedVarInt().value_or(0);
		const bool slotsCountAvailable = stream.canReadCollection(slotsCount);
		for (std::size_t i = 0; i < slotsCount && slotsCountAvailable && !stream.hasOverflowed(); ++i) {
		    slots.push_back(stream.readObject<ItemStackResponseSlotInfo>());
		}
		return std::make_shared<ItemStackResponseContainerInfo>(containerName, slots);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		containerName->write(stream);
		stream.writeUnsignedVarInt(slots.size());
		for (const auto& slot : slots) {
			slot->write(stream);
		}
    }
};
