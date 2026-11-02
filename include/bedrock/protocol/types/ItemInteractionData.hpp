#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
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

    static std::shared_ptr<ItemInteractionData> create(ReadOnlyBinaryStream& stream) {
		auto requestId = stream.readVarInt().value_or(0);
		auto requestChangedSlots = std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>>();
		if (requestId != 0) {
			auto len = stream.readUnsignedVarInt().value_or(0);
			if (len > ReadOnlyBinaryStream::MaxCollectionSize) {
			    throw std::runtime_error("protocol collection is too large");
			}
			for (std::size_t i = 0; (i < len); ++i) {
				requestChangedSlots.push_back(stream.readObject<InventoryTransactionChangedSlotsHack>());
			}
		}
		auto transactionData = std::make_shared<UseItemTransactionData>();
		transactionData->read(stream);
		return std::make_shared<ItemInteractionData>(requestId, requestChangedSlots, transactionData);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
