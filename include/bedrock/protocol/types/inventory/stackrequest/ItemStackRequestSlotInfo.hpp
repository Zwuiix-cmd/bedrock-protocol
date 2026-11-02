#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/inventory/FullContainerName.hpp>

class ItemStackRequestSlotInfo {
public:
    std::shared_ptr<FullContainerName> containerName;
    uint8_t slotId;
    int32_t stackId;

    ItemStackRequestSlotInfo() = default;
    explicit ItemStackRequestSlotInfo(
        const std::shared_ptr<FullContainerName>& containerNameValue,
        const uint8_t& slotIdValue,
        const int32_t& stackIdValue
    ) :
        containerName(containerNameValue),
        slotId(slotIdValue),
        stackId(stackIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerName = std::make_shared<FullContainerName>();
        containerName->read(stream);
        slotId = stream.readUnsignedByte().value_or(0);
        stackId = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		containerName->write(stream);
		stream.writeUnsignedByte(slotId);
		stream.writeVarInt(stackId);
    }
};
