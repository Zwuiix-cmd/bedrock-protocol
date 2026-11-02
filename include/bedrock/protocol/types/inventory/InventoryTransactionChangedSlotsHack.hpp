#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

namespace bedrock::protocol {


class InventoryTransactionChangedSlotsHack {
public:
    uint8_t containerId;
    std::vector<uint8_t> changedSlotIndexes;

    InventoryTransactionChangedSlotsHack() = default;
    explicit InventoryTransactionChangedSlotsHack(const uint8_t& containerIdValue, const std::vector<uint8_t>& changedSlotIndexesValue) : containerId(containerIdValue), changedSlotIndexes(changedSlotIndexesValue) {}

    static std::shared_ptr<InventoryTransactionChangedSlotsHack> create(ReadOnlyBinaryStream& stream) {
		auto containerId = stream.readUnsignedByte().value_or(0);
		auto changedSlots = std::vector<uint8_t>();
		auto changedSlotsCount = stream.readUnsignedVarInt().value_or(0);
		const bool changedSlotsCountAvailable = stream.canReadCollection(changedSlotsCount);
		for (std::size_t i = 0; i < changedSlotsCount && changedSlotsCountAvailable && !stream.hasOverflowed(); ++i) {
		    changedSlots.push_back(stream.readUnsignedByte().value_or(0));
		}
		return std::make_shared<InventoryTransactionChangedSlotsHack>(static_cast<int32_t>(containerId), changedSlots);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(containerId);
		stream.writeUnsignedVarInt(changedSlotIndexes.size());
		for (const auto& index : changedSlotIndexes) {
			stream.writeUnsignedByte(index);
		}
    }
};

} // namespace bedrock::protocol
