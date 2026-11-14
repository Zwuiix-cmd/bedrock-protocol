#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <exception>
#include <variant>
#include <vector>

#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>
namespace bedrock::protocol {


class PersonaPieceTintColor {
public:
    static constexpr int64_t EXPECTED_COLOR_COUNT = 4;

    PersonaSkinPieceType pieceType;
    std::vector<std::monostate> colors;

    PersonaPieceTintColor() = default;
    explicit PersonaPieceTintColor(const PersonaSkinPieceType& pieceTypeValue, const std::vector<std::monostate>& colorsValue) : pieceType(pieceTypeValue), colors(colorsValue) {
		if (colors.size() != EXPECTED_COLOR_COUNT) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(
            stream.readString().value_or("")
        ).value_or(PersonaSkinPieceType::Unknown);
        colors = std::monostate{};
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_integer(pieceType));
        (void)(colors);
    }
};

} // namespace bedrock::protocol
