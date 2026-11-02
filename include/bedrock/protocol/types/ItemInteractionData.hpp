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
#include <bedrock/protocol/types/inventory/UseItemTransactionData.hpp>
#include <bedrock/protocol/types/inventory/InventoryTransactionChangedSlotsHack.hpp>

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
