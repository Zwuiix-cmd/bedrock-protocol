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
#include <bedrock/protocol/types/inventory/stackrequest/ItemStackRequestAction.hpp>

class CraftRecipeOptionalStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 15;

    int32_t recipeId;
    int32_t filterStringIndex;

    CraftRecipeOptionalStackRequestAction() = default;
    virtual ~CraftRecipeOptionalStackRequestAction() = default;

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        filterStringIndex = stream.readInt().value_or(0);
        recipeId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(recipeId);
		stream.writeInt(filterStringIndex);
    }
};
