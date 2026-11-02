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
#include <bedrock/protocol/types/ArmorSlot.hpp>
#include <bedrock/protocol/types/ArmorSlot.hpp>
#include <bedrock/protocol/types/ArmorSlot.hpp>

class ArmorSlotAndDamagePair {
public:
    ArmorSlot slot;
    uint16_t damage;

    ArmorSlotAndDamagePair() = default;
    explicit ArmorSlotAndDamagePair(const ArmorSlot& slotValue, const uint16_t& damageValue) : slot(slotValue), damage(damageValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        damage = stream.readUnsignedShort().value_or(0);
        slot = static_cast<ArmorSlot>(stream.readVarInt().value_or(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(static_cast<std::underlying_type_t<std::remove_cvref_t<decltype(slot)>>>(slot));
		stream.writeUnsignedShort(damage);
    }
};
