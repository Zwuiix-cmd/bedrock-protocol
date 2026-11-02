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

class ItemStackRequestActionType {
public:
    static constexpr int64_t TAKE = 0;
    static constexpr int64_t PLACE = 1;
    static constexpr int64_t SWAP = 2;
    static constexpr int64_t DROP = 3;
    static constexpr int64_t DESTROY = 4;
    static constexpr int64_t CRAFTING_CONSUME_INPUT = 5;
    static constexpr int64_t CRAFTING_CREATE_SPECIFIC_RESULT = 6;
    static constexpr int64_t LAB_TABLE_COMBINE = 7;
    static constexpr int64_t BEACON_PAYMENT = 8;
    static constexpr int64_t MINE_BLOCK = 9;
    static constexpr int64_t CRAFTING_RECIPE = 10;
    static constexpr int64_t CRAFTING_RECIPE_AUTO = 11;
    static constexpr int64_t CREATIVE_CREATE = 12;
    static constexpr int64_t CRAFTING_RECIPE_OPTIONAL = 13;
    static constexpr int64_t CRAFTING_GRINDSTONE = 14;
    static constexpr int64_t CRAFTING_LOOM = 15;
    static constexpr int64_t CRAFTING_RESERVED = 16;
    static constexpr int64_t CRAFTING_NON_IMPLEMENTED_DEPRECATED_ASK_TY_LAING = 17;
    static constexpr int64_t CRAFTING_RESULTS_DEPRECATED_ASK_TY_LAING = 18;


    ItemStackRequestActionType() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
