#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
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

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerName = std::make_shared<FullContainerName>();
        containerName->read(stream);
        slots = stream.readObjectList<ItemStackResponseSlotInfo>();
    }

    virtual void write(BinaryStream& stream) const {
		containerName->write(stream);
		stream.writeUnsignedVarInt(slots.size());
		for (const auto& slot : slots) {
			slot->write(stream);
		}
    }
};
