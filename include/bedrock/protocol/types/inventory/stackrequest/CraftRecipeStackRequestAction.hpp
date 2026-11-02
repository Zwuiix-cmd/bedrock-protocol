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

class CraftRecipeStackRequestAction : public ItemStackRequestAction {
public:
    static constexpr int64_t ID = 12;

    int32_t recipeId;
    uint8_t repetitions;

    CraftRecipeStackRequestAction() = default;
    virtual ~CraftRecipeStackRequestAction() = default;
    explicit CraftRecipeStackRequestAction(const int32_t& recipeIdValue, const uint8_t& repetitionsValue) : recipeId(recipeIdValue), repetitions(repetitionsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        repetitions = stream.readUnsignedByte().value_or(0);
        recipeId = stream.readInt().value_or(0);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(recipeId);
		stream.writeUnsignedByte(repetitions);
    }
};
