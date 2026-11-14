#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <variant>

#include <bedrock/protocol/types/recipe/MolangItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/StringIdMetaItemDescriptor.hpp>
#include <bedrock/protocol/types/recipe/TagItemDescriptor.hpp>
namespace bedrock::protocol {


class RecipeIngredient {
public:
    std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>> descriptor;
    uint16_t count;

    RecipeIngredient() = default;
    explicit RecipeIngredient(
        const std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>& descriptorValue,
        const uint16_t& countValue
    ) :
        descriptor(descriptorValue),
        count(countValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        descriptor = stream.readOptionalVariant<std::variant<std::monostate, std::shared_ptr<StringIdMetaItemDescriptor>, std::shared_ptr<TagItemDescriptor>, std::shared_ptr<MolangItemDescriptor>>>([&]() { return stream.readObject<StringIdMetaItemDescriptor>(); });
        count = stream.readUnsignedShort().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        std::visit([&](const auto& alternative) { using Alternative = std::remove_cvref_t<decltype(alternative)>; if constexpr (std::is_same_v<Alternative, std::shared_ptr<StringIdMetaItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<TagItemDescriptor>>) { alternative->write(stream); } else if constexpr (std::is_same_v<Alternative, std::shared_ptr<MolangItemDescriptor>>) { alternative->write(stream); } }, descriptor);
        stream.writeUnsignedShort(count);
    }
};

} // namespace bedrock::protocol
