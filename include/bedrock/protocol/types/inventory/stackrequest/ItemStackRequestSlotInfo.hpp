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
        slotId = stream.readUnsignedByte().value_or(0);
        containerName = std::make_shared<FullContainerName>();
        containerName->read(stream);
        stackId = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		containerName->write(stream);
		stream.writeUnsignedByte(slotId);
		stream.writeInt(stackId);
    }
};
