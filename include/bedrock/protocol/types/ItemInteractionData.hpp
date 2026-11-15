#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>

#include <bedrock/protocol/types/inventory/UseItemTransactionData.hpp>
#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>
namespace bedrock::protocol {


class ItemInteractionData {
public:
    int32_t requestId;
    std::optional<std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>>> requestChangedSlots;
    std::shared_ptr<UseItemTransactionData> transactionData;

    ItemInteractionData() = default;
    explicit ItemInteractionData(
        const int32_t& requestIdValue,
        const std::optional<std::vector<std::shared_ptr<InventoryTransactionChangedSlotsHack>>>& requestChangedSlotsValue,
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
		stream.writeOptional(requestChangedSlots, [&](const auto& list) {
			stream.writeList(list, [&](const auto& v) {
				v->write(stream);
			});
		});
		transactionData->write(stream);
    }
};

} // namespace bedrock::protocol
