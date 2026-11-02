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
#include <variant>
#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>

class PersonaPieceTintColor {
public:
    static constexpr int64_t EXPECTED_COLOR_COUNT = 4;

    PersonaSkinPieceType pieceType;
    std::vector<std::monostate> colors;

    PersonaPieceTintColor() = default;
    explicit PersonaPieceTintColor(const PersonaSkinPieceType& pieceTypeValue, const std::vector<std::monostate>& colorsValue) : pieceType(pieceTypeValue), colors(colorsValue) {
		if ((colors.size() != EXPECTED_COLOR_COUNT)) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = static_cast<PersonaSkinPieceType>(stream.readVarInt().value_or(0));
        colors = std::monostate{};
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVarInt(static_cast<int32_t>(pieceType));
        (void)(colors);
    }
};
