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

class PersonaPieceTintColor {
public:
    enum class PieceType : uint8_t {
        PersonaEyes = 0,
        PersonaHair = 1,
        PersonaMouth = 2,
    };

    PieceType pieceType;
    std::vector<std::string> colors;

    PersonaPieceTintColor() = default;
    explicit PersonaPieceTintColor(const PieceType& pieceTypeValue, const std::vector<std::string>& colorsValue) : pieceType(pieceTypeValue), colors(colorsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto pieceTypeRaw = stream.readString().value_or("");
        pieceType = magic_enum::enum_cast<PieceType>(
            std::string_view(pieceTypeRaw)
        ).value_or(PieceType::PersonaEyes);
        colors = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_name(pieceType));
        stream.writeList(colors, [&](const auto& item) {
            stream.writeString(item);
        });
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<PersonaPieceTintColor::PieceType>(PersonaPieceTintColor::PieceType value) noexcept {
    switch (value) {
        case PersonaPieceTintColor::PieceType::PersonaEyes: return "persona_eyes";
        case PersonaPieceTintColor::PieceType::PersonaHair: return "persona_hair";
        case PersonaPieceTintColor::PieceType::PersonaMouth: return "persona_mouth";
        default: return invalid_tag;
    }
}

}
