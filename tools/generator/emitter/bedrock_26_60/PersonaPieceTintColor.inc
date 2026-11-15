<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(
            stream.readString().value_or("")
        ).value_or(PersonaSkinPieceType::Unknown);
        colors = std::monostate{};
    }
BEFORE,
<<<'AFTER'
    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(stream.readString().value_or("")).value_or(PersonaSkinPieceType::Unknown);
        for (auto& color : colors) {
            color = stream.readUnsignedInt().value_or(0);
        }
    }
AFTER],
    [<<<'BEFORE'
    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_integer(pieceType));
        (void)(colors);
    }
BEFORE,
<<<'AFTER'
    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_name(pieceType));
        for (const auto color : colors) {
            stream.writeUnsignedInt(color);
        }
    }
AFTER],
    [<<<'BEFORE'
#include <cstdint>
BEFORE,
<<<'AFTER'
#include <array>
#include <cstdint>
AFTER],
    [<<<'BEFORE'
std::vector<std::monostate> colors;
BEFORE,
<<<'AFTER'
std::array<uint32_t, EXPECTED_COLOR_COUNT> colors{};
AFTER],
    [<<<'BEFORE'
const std::vector<std::monostate>& colorsValue
BEFORE,
<<<'AFTER'
const std::array<uint32_t, EXPECTED_COLOR_COUNT>& colorsValue
AFTER],
];
