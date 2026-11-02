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
#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>

class NetworkInventoryAction {
public:
    enum class Source : int64_t {
        Container = 0,
        Global = 1,
        World = 2,
        Creative = 3,
        Todo = 99999,
        TypeCraftingResult = -4,
        TypeCraftingUseIngredient = -5,
        TypeAnvilResult = -12,
        TypeAnvilOutput = -13,
        TypeEnchantOutput = -17,
        TypeTradingInput1 = -20,
        TypeTradingInput2 = -21,
        TypeTradingUseInputs = -22,
        TypeTradingOutput = -23,
        TypeBeacon = -24,
    };

    enum class Action : uint8_t {
        MagicSlotCreativeDeleteItem = 0,
        MagicSlotCreativeCreateItem = 1,
        MagicSlotDropItem = 0,
        MagicSlotPickupItem = 1,
    };

    Source sourceType;
    std::optional<int32_t> windowId;
    std::optional<int32_t> sourceFlags;
    uint32_t inventorySlot;
    NetworkItemStack oldItem;
    NetworkItemStack newItem;

    NetworkInventoryAction() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        stream.writeUnsignedVarInt(magic_enum::enum_integer(sourceType));
        stream.writeUnsignedVarInt(inventorySlot);
        stream.writeOptional(windowId, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        stream.writeOptional(sourceFlags, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        oldItem.write(stream);
        newItem.write(stream);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(magic_enum::enum_integer(sourceType));
        stream.writeUnsignedVarInt(inventorySlot);
        stream.writeOptional(windowId, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        stream.writeOptional(sourceFlags, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        oldItem.write(stream);
        newItem.write(stream);
    }
};
