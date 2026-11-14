#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <array>
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
    std::array<uint32_t, EXPECTED_COLOR_COUNT> colors{};

    PersonaPieceTintColor() = default;
    explicit PersonaPieceTintColor(const PersonaSkinPieceType& pieceTypeValue, const std::array<uint32_t, EXPECTED_COLOR_COUNT>& colorsValue) : pieceType(pieceTypeValue), colors(colorsValue) {
		if (colors.size() != EXPECTED_COLOR_COUNT) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(stream.readString().value_or("")).value_or(PersonaSkinPieceType::Unknown);
        for (auto& color : colors) {
            color = stream.readUnsignedInt().value_or(0);
        }
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_name(pieceType));
        for (const auto color : colors) {
            stream.writeUnsignedInt(color);
        }
    }
};

} // namespace bedrock::protocol
