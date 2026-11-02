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
    static constexpr int64_t SOURCE_CONTAINER = 0;
    static constexpr int64_t SOURCE_GLOBAL = 1;
    static constexpr int64_t SOURCE_WORLD = 2;
    static constexpr int64_t SOURCE_CREATIVE = 3;
    static constexpr int64_t SOURCE_TODO = 99999;
    static constexpr int64_t SOURCE_TYPE_CRAFTING_RESULT = -4;
    static constexpr int64_t SOURCE_TYPE_CRAFTING_USE_INGREDIENT = -5;
    static constexpr int64_t SOURCE_TYPE_ANVIL_RESULT = -12;
    static constexpr int64_t SOURCE_TYPE_ANVIL_OUTPUT = -13;
    static constexpr int64_t SOURCE_TYPE_ENCHANT_OUTPUT = -17;
    static constexpr int64_t SOURCE_TYPE_TRADING_INPUT_1 = -20;
    static constexpr int64_t SOURCE_TYPE_TRADING_INPUT_2 = -21;
    static constexpr int64_t SOURCE_TYPE_TRADING_USE_INPUTS = -22;
    static constexpr int64_t SOURCE_TYPE_TRADING_OUTPUT = -23;
    static constexpr int64_t SOURCE_TYPE_BEACON = -24;
    static constexpr int64_t ACTION_MAGIC_SLOT_CREATIVE_DELETE_ITEM = 0;
    static constexpr int64_t ACTION_MAGIC_SLOT_CREATIVE_CREATE_ITEM = 1;
    static constexpr int64_t ACTION_MAGIC_SLOT_DROP_ITEM = 0;
    static constexpr int64_t ACTION_MAGIC_SLOT_PICKUP_ITEM = 1;

    uint32_t sourceType;
    std::optional<int32_t> windowId;
    std::optional<int32_t> sourceFlags;
    uint32_t inventorySlot;
    NetworkItemStack oldItem;
    NetworkItemStack newItem;

    NetworkInventoryAction() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        stream.writeUnsignedVarInt(sourceType);
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
        stream.writeUnsignedVarInt(sourceType);
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
