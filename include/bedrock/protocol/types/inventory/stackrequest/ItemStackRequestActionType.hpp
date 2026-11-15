#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class ItemStackRequestActionType {
public:
    enum class Crafting : uint8_t {
        ConsumeInput = 5,
        CreateSpecificResult = 6,
        Recipe = 10,
        RecipeAuto = 11,
        RecipeOptional = 13,
        Grindstone = 14,
        Loom = 15,
        Reserved = 16,
        NonImplementedDeprecatedAskTyLaing = 17,
        ResultsDeprecatedAskTyLaing = 18,
    };

    static constexpr int64_t TAKE = 0;
    static constexpr int64_t PLACE = 1;
    static constexpr int64_t SWAP = 2;
    static constexpr int64_t DROP = 3;
    static constexpr int64_t DESTROY = 4;
    static constexpr int64_t LAB_TABLE_COMBINE = 7;
    static constexpr int64_t BEACON_PAYMENT = 8;
    static constexpr int64_t MINE_BLOCK = 9;
    static constexpr int64_t CREATIVE_CREATE = 12;


    ItemStackRequestActionType() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};

} // namespace bedrock::protocol
