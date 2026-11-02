#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class UIInventorySlotOffset {
public:
    static constexpr int64_t CURSOR = 0;
    static constexpr int64_t STONE_CUTTER_INPUT = 3;
    static constexpr int64_t MATERIAL_REDUCER_INPUT = 8;
    static constexpr int64_t BEACON_PAYMENT = 27;
    static constexpr int64_t CREATED_ITEM_OUTPUT = 50;


    UIInventorySlotOffset() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        (void)stream;
    }

    virtual void write(BinaryStream& stream) const {
        (void)stream;
    }
};
