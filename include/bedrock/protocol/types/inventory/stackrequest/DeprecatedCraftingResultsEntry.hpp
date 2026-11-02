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
#include <variant>
#include <bedrock/protocol/types/recipe/MolangItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/StringIdMetaItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/TagItemDescriptor.hpp>

class DeprecatedCraftingResultsEntry {
public:
    std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>> descriptor;
    uint16_t count;
    uint32_t blockRuntimeId;
    std::string rawExtraData;

    DeprecatedCraftingResultsEntry() = default;
    explicit DeprecatedCraftingResultsEntry(
        const std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>& descriptorValue,
        const uint16_t& countValue,
        const uint32_t& blockRuntimeIdValue,
        const std::string& rawExtraDataValue
    ) :
        descriptor(descriptorValue),
        count(countValue),
        blockRuntimeId(blockRuntimeIdValue),
        rawExtraData(rawExtraDataValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        count = stream.readUnsignedShort().value_or(0);
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        rawExtraData = stream.readString().value_or("");
        descriptor = ([&]() { auto value = stream.readOptional([&]() { return ([&]() {
            auto value = std::make_shared<StringIdMetaItemDescriptor>();
            value->read(stream);
            return value;
        }()); }); return value ? std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>{*value} : std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>{std::monostate{}}; }());
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedShort(count);
        stream.writeUnsignedVarInt(blockRuntimeId);
        stream.writeString(rawExtraData);
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<StringIdMetaItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<TagItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<MolangItemDescriptor>>) { alternative->write(stream); } }, descriptor);
    }
};
