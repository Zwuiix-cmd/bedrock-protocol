#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/Enchant.hpp>

class EnchantOption {
public:
    uint32_t cost;
    uint32_t slotFlags;
    std::vector<std::shared_ptr<Enchant>> equipActivatedEnchantments;
    std::vector<std::shared_ptr<Enchant>> heldActivatedEnchantments;
    std::vector<std::shared_ptr<Enchant>> selfActivatedEnchantments;
    std::string name;
    uint32_t optionId;

    EnchantOption() = default;
    explicit EnchantOption(
        const uint32_t& costValue,
        const uint32_t& slotFlagsValue,
        const std::vector<std::shared_ptr<Enchant>>& equipActivatedEnchantmentsValue,
        const std::vector<std::shared_ptr<Enchant>>& heldActivatedEnchantmentsValue,
        const std::vector<std::shared_ptr<Enchant>>& selfActivatedEnchantmentsValue,
        const std::string& nameValue,
        const uint32_t& optionIdValue
    ) :
        cost(costValue),
        slotFlags(slotFlagsValue),
        equipActivatedEnchantments(equipActivatedEnchantmentsValue),
        heldActivatedEnchantments(heldActivatedEnchantmentsValue),
        selfActivatedEnchantments(selfActivatedEnchantmentsValue),
        name(nameValue),
        optionId(optionIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        cost = stream.readUnsignedVarInt().value_or(0);
        slotFlags = stream.readUnsignedInt().value_or(0);
        equipActivatedEnchantments = stream.readObjectList<Enchant>();
        heldActivatedEnchantments = stream.readObjectList<Enchant>();
        selfActivatedEnchantments = stream.readObjectList<Enchant>();
        name = stream.readString().value_or("");
        optionId = stream.readUnsignedVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(cost);
        stream.writeUnsignedInt(slotFlags);
        stream.writeList(equipActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(heldActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(selfActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeString(name);
        stream.writeUnsignedVarInt(optionId);
    }
};
