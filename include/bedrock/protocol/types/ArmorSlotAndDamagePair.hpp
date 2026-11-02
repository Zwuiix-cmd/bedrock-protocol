#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

#include <bedrock/protocol/types/ArmorSlot.hpp>

class ArmorSlotAndDamagePair {
public:
    ArmorSlot slot;
    uint16_t damage;

    ArmorSlotAndDamagePair() = default;
    explicit ArmorSlotAndDamagePair(const ArmorSlot& slotValue, const uint16_t& damageValue) : slot(slotValue), damage(damageValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        damage = stream.readUnsignedShort().value_or(0);
        slot = magic_enum::enum_cast<ArmorSlot>(
            stream.readVarInt().value_or(0)
        ).value_or(ArmorSlot::Head);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedByte(magic_enum::enum_integer(slot));
		stream.writeUnsignedShort(damage);
    }
};
