#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string>
#include <string_view>


class PersonaSkinPiece {
public:
    enum class PieceId : uint8_t {
        TypePersonaBody = 0,
        TypePersonaBottom = 1,
        TypePersonaEyes = 2,
        TypePersonaFacialHair = 3,
        TypePersonaFeet = 4,
        TypePersonaHair = 5,
        TypePersonaMouth = 6,
        TypePersonaSkeleton = 7,
        TypePersonaSkin = 8,
        TypePersonaTop = 9,
    };

    enum class PieceType : uint8_t {
        PersonaBody = 0,
        PersonaBottom = 1,
        PersonaEyes = 2,
        PersonaFacialHair = 3,
        PersonaFeet = 4,
        PersonaHair = 5,
        PersonaMouth = 6,
        PersonaSkeleton = 7,
        PersonaSkin = 8,
        PersonaTop = 9,
    };

    PieceId pieceId;
    PieceType pieceType;
    std::string packId;
    bool isDefaultPiece;
    std::string productId;

    PersonaSkinPiece() = default;
    explicit PersonaSkinPiece(
        const PieceId& pieceIdValue,
        const PieceType& pieceTypeValue,
        const std::string& packIdValue,
        const bool& isDefaultPieceValue,
        const std::string& productIdValue
    ) :
        pieceId(pieceIdValue),
        pieceType(pieceTypeValue),
        packId(packIdValue),
        isDefaultPiece(isDefaultPieceValue),
        productId(productIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        const auto pieceIdRaw = stream.readString().value_or("");
        pieceId = magic_enum::enum_cast<PieceId>(
            std::string_view(pieceIdRaw)
        ).value_or(PieceId::TypePersonaBody);
        const auto pieceTypeRaw = stream.readString().value_or("");
        pieceType = magic_enum::enum_cast<PieceType>(
            std::string_view(pieceTypeRaw)
        ).value_or(PieceType::PersonaBody);
        packId = stream.readString().value_or("");
        isDefaultPiece = stream.readBool().value_or(false);
        productId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(magic_enum::enum_name(pieceId));
        stream.writeString(magic_enum::enum_name(pieceType));
        stream.writeString(packId);
        stream.writeBool(isDefaultPiece);
        stream.writeString(productId);
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<PersonaSkinPiece::PieceId>(PersonaSkinPiece::PieceId value) noexcept {
    switch (value) {
        case PersonaSkinPiece::PieceId::TypePersonaBody: return "persona_body";
        case PersonaSkinPiece::PieceId::TypePersonaBottom: return "persona_bottom";
        case PersonaSkinPiece::PieceId::TypePersonaEyes: return "persona_eyes";
        case PersonaSkinPiece::PieceId::TypePersonaFacialHair: return "persona_facial_hair";
        case PersonaSkinPiece::PieceId::TypePersonaFeet: return "persona_feet";
        case PersonaSkinPiece::PieceId::TypePersonaHair: return "persona_hair";
        case PersonaSkinPiece::PieceId::TypePersonaMouth: return "persona_mouth";
        case PersonaSkinPiece::PieceId::TypePersonaSkeleton: return "persona_skeleton";
        case PersonaSkinPiece::PieceId::TypePersonaSkin: return "persona_skin";
        case PersonaSkinPiece::PieceId::TypePersonaTop: return "persona_top";
        default: return invalid_tag;
    }
}

template <> constexpr customize_t enum_name<PersonaSkinPiece::PieceType>(PersonaSkinPiece::PieceType value) noexcept {
    switch (value) {
        case PersonaSkinPiece::PieceType::PersonaBody: return "persona_body";
        case PersonaSkinPiece::PieceType::PersonaBottom: return "persona_bottom";
        case PersonaSkinPiece::PieceType::PersonaEyes: return "persona_eyes";
        case PersonaSkinPiece::PieceType::PersonaFacialHair: return "persona_facial_hair";
        case PersonaSkinPiece::PieceType::PersonaFeet: return "persona_feet";
        case PersonaSkinPiece::PieceType::PersonaHair: return "persona_hair";
        case PersonaSkinPiece::PieceType::PersonaMouth: return "persona_mouth";
        case PersonaSkinPiece::PieceType::PersonaSkeleton: return "persona_skeleton";
        case PersonaSkinPiece::PieceType::PersonaSkin: return "persona_skin";
        case PersonaSkinPiece::PieceType::PersonaTop: return "persona_top";
        default: return invalid_tag;
    }
}

}
