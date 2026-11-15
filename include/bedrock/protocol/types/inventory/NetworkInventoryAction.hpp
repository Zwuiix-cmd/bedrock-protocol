#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <optional>

#include <bedrock/protocol/types/inventory/NetworkItemStack.hpp>
namespace bedrock::protocol {


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
    std::optional<int8_t> windowId;
    std::optional<uint32_t> sourceFlags;
    uint32_t inventorySlot;
    NetworkItemStack oldItem;
    NetworkItemStack newItem;

    NetworkInventoryAction() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
		sourceType = magic_enum::enum_cast<Source>(
			stream.readUnsignedVarInt().value_or(0)
		).value_or(Source::Container);
		windowId = stream.readOptional([&]() {
			return stream.readByte().value_or(0);
		});
		sourceFlags = stream.readOptional([&]() {
			return stream.readUnsignedVarInt().value_or(0);
		});
		inventorySlot = stream.readUnsignedVarInt().value_or(0);
		oldItem = stream.readValue<NetworkItemStack>();
		newItem = stream.readValue<NetworkItemStack>();
		return;
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(magic_enum::enum_integer(sourceType));
		stream.writeOptional(windowId, [&](const auto& value) {
			stream.writeByte(value);
		});
		stream.writeOptional(sourceFlags, [&](const auto& value) {
			stream.writeUnsignedVarInt(value);
		});
		stream.writeUnsignedVarInt(inventorySlot);
		oldItem.write(stream);
		newItem.write(stream);
    }
};

} // namespace bedrock::protocol
