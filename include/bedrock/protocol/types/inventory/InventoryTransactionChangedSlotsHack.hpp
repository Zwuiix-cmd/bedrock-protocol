#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <vector>


class InventoryTransactionChangedSlotsHack {
public:
    uint8_t containerId;
    std::vector<int32_t> changedSlotIndexes;

    InventoryTransactionChangedSlotsHack() = default;
    explicit InventoryTransactionChangedSlotsHack(const uint8_t& containerIdValue, const std::vector<int32_t>& changedSlotIndexesValue) : containerId(containerIdValue), changedSlotIndexes(changedSlotIndexesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        containerId = stream.readUnsignedByte().value_or(0);
        changedSlotIndexes = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(containerId);
		stream.writeUnsignedVarInt(changedSlotIndexes.size());
		for (const auto& index : changedSlotIndexes) {
			stream.writeUnsignedByte(index);
		}
    }
};
