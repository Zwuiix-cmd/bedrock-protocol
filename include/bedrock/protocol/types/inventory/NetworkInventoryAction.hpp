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
    int32_t windowId;
    uint32_t sourceFlags = 0;
    uint32_t inventorySlot;
    NetworkItemStack oldItem;
    NetworkItemStack newItem;

    NetworkInventoryAction() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
		sourceType = magic_enum::enum_cast<Source>(
			stream.readUnsignedVarInt().value_or(0)
		).value_or(Source::Container);
		switch (sourceType) {
			case Source::Container: {
				windowId = stream.readVarInt().value_or(0);
				break;
			}
			case Source::World: {
				sourceFlags = stream.readUnsignedVarInt().value_or(0);
				break;
			}
			case Source::Creative: {
				break;
			}
			case Source::Todo: {
				windowId = stream.readVarInt().value_or(0);
				break;
			}
			default: {
				throw std::exception{};
			}
		}
		inventorySlot = stream.readUnsignedVarInt().value_or(0);
		oldItem.read(stream);
		newItem.read(stream);
		return;
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(magic_enum::enum_integer(sourceType));
		switch (sourceType) {
			case Source::Container: {
				stream.writeVarInt(windowId);
				break;
			}
			case Source::World: {
				stream.writeUnsignedVarInt(sourceFlags);
				break;
			}
			case Source::Creative: {
				break;
			}
			case Source::Todo: {
				stream.writeVarInt(windowId);
				break;
			}
			default: {
				throw std::exception{};
			}
		}
		stream.writeUnsignedVarInt(inventorySlot);
		oldItem.write(stream);
		newItem.write(stream);
    }
};
