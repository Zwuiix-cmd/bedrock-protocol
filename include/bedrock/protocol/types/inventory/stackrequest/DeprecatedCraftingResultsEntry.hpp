#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>

#include <bedrock/protocol/types/recipe/MolangItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/StringIdMetaItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/TagItemDescriptor.hpp>
namespace bedrock::protocol {


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
        descriptor = stream.readOptionalVariant<std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>>([&]() { return stream.readObject<StringIdMetaItemDescriptor>(); });
        count = stream.readUnsignedShort().value_or(0);
        blockRuntimeId = stream.readUnsignedVarInt().value_or(0);
        rawExtraData = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<StringIdMetaItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<TagItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<MolangItemDescriptor>>) { alternative->write(stream); } }, descriptor);
        stream.writeUnsignedShort(count);
        stream.writeUnsignedVarInt(blockRuntimeId);
        stream.writeString(rawExtraData);
    }
};

} // namespace bedrock::protocol
