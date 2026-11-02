#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>

#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class RecipeIngredient {
public:
    std::optional<std::shared_ptr<ItemDescriptor>> descriptor;
    int32_t count;

    RecipeIngredient() = default;
    explicit RecipeIngredient(const std::optional<std::shared_ptr<ItemDescriptor>>& descriptorValue, const int32_t& countValue) : descriptor(descriptorValue), count(countValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        descriptor = stream.readOptional([&]() {
            return ItemDescriptor::create(stream);
        });
        count = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeOptional(descriptor, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeVarInt(count);
    }
};
