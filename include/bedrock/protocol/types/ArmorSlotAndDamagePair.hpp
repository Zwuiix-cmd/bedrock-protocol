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
        slot = magic_enum::enum_cast<ArmorSlot>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<ArmorSlot>(0));
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVarInt(magic_enum::enum_integer(slot));
		stream.writeUnsignedShort(damage);
    }
};
