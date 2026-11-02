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
#include <bedrock/protocol/types/Enchant.hpp>

class EnchantOption {
public:
    uint32_t cost;
    uint32_t slotFlags;
    std::vector<std::shared_ptr<Enchant>> equipActivatedEnchantments;
    std::vector<std::shared_ptr<Enchant>> heldActivatedEnchantments;
    std::vector<std::shared_ptr<Enchant>> selfActivatedEnchantments;
    std::string name;
    int32_t optionId;

    EnchantOption() = default;
    explicit EnchantOption(
        const uint32_t& costValue,
        const uint32_t& slotFlagsValue,
        const std::vector<std::shared_ptr<Enchant>>& equipActivatedEnchantmentsValue,
        const std::vector<std::shared_ptr<Enchant>>& heldActivatedEnchantmentsValue,
        const std::vector<std::shared_ptr<Enchant>>& selfActivatedEnchantmentsValue,
        const std::string& nameValue,
        const int32_t& optionIdValue
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
        name = stream.readString().value_or("");
        equipActivatedEnchantments = stream.readObjectList<Enchant>();
        heldActivatedEnchantments = stream.readObjectList<Enchant>();
        selfActivatedEnchantments = stream.readObjectList<Enchant>();
        optionId = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(cost);
        stream.writeUnsignedInt(slotFlags);
        stream.writeString(name);
        stream.writeList(equipActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(heldActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(selfActivatedEnchantments, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeInt(optionId);
    }
};
