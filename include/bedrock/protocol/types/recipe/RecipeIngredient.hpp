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
#include <bedrock/protocol/types/recipe/ItemDescriptor.hpp>

class RecipeIngredient {
public:
    std::optional<std::shared_ptr<ItemDescriptor>> descriptor;
    int32_t count;

    RecipeIngredient() = default;
    explicit RecipeIngredient(const std::optional<std::shared_ptr<ItemDescriptor>>& descriptorValue, const int32_t& countValue) : descriptor(descriptorValue), count(countValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        count = stream.readVarInt().value_or(0);
        descriptor = stream.readOptional([&]() {
            return ItemDescriptor::decode(stream);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(count);
        stream.writeOptional(descriptor, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
    }
};
