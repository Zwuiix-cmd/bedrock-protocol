#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>
#include <string>

namespace bedrock::protocol {


class ItemStackResponseSlotInfo {
public:
    uint8_t slot;
    uint8_t hotbarSlot;
    uint8_t count;
    std::optional<int32_t> itemStackId;
    std::string customName;
    std::optional<std::string> filteredCustomName;
    int32_t durabilityCorrection;

    ItemStackResponseSlotInfo() = default;
    explicit ItemStackResponseSlotInfo(
        const uint8_t& slotValue,
        const uint8_t& hotbarSlotValue,
        const uint8_t& countValue,
        const std::optional<int32_t>& itemStackIdValue,
        const std::string& customNameValue,
        const std::optional<std::string>& filteredCustomNameValue,
        const int32_t& durabilityCorrectionValue
    ) :
        slot(slotValue),
        hotbarSlot(hotbarSlotValue),
        count(countValue),
        itemStackId(itemStackIdValue),
        customName(customNameValue),
        filteredCustomName(filteredCustomNameValue),
        durabilityCorrection(durabilityCorrectionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        slot = stream.readUnsignedByte().value_or(0);
        hotbarSlot = stream.readUnsignedByte().value_or(0);
        count = stream.readUnsignedByte().value_or(0);
        itemStackId = stream.readOptional([&]() {
            return stream.readVarInt().value_or(0);
        });
        customName = stream.readString().value_or("");
        filteredCustomName = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        durabilityCorrection = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(slot);
		stream.writeUnsignedByte(hotbarSlot);
		stream.writeUnsignedByte(count);
		stream.writeOptional(itemStackId, [&](const auto& value) {
			stream.writeVarInt(value);
		});
		stream.writeString(customName);
		stream.writeOptional(filteredCustomName, [&](const auto& value) {
			stream.writeString(value);
		});
		stream.writeVarInt(durabilityCorrection);
    }
};

} // namespace bedrock::protocol
