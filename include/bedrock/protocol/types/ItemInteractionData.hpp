#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <vector>

#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>
#include <bedrock/protocol/types/inventory/UseItemTransactionData.hpp>

class ItemInteractionData {
public:
    int32_t requestId;
    std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>> requestChangedSlots;
    std::shared_ptr<UseItemTransactionData> transactionData;

    ItemInteractionData() = default;
    explicit ItemInteractionData(
        const int32_t& requestIdValue,
        const std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>>& requestChangedSlotsValue,
        const std::shared_ptr<UseItemTransactionData>& transactionDataValue
    ) :
        requestId(requestIdValue),
        requestChangedSlots(requestChangedSlotsValue),
        transactionData(transactionDataValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        requestId = stream.readVarInt().value_or(0);
        requestChangedSlots = stream.readObjectList<InventoryTransactionChangedSlotsHack>();
        transactionData = std::make_shared<UseItemTransactionData>();
        transactionData->read(stream);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(requestId);
		if (requestId != 0) {
			stream.writeUnsignedVarInt(requestChangedSlots.size());
			for (const auto& changedSlot : requestChangedSlots) {
				changedSlot->write(stream);
			}
		}
		transactionData->write(stream);
    }
};
