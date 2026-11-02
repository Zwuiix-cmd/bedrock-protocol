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

class PersonaSkinPiece {
public:
    static constexpr std::string_view PIECE_TYPE_PERSONA_BODY = "persona_body";
    static constexpr std::string_view PIECE_TYPE_PERSONA_BOTTOM = "persona_bottom";
    static constexpr std::string_view PIECE_TYPE_PERSONA_EYES = "persona_eyes";
    static constexpr std::string_view PIECE_TYPE_PERSONA_FACIAL_HAIR = "persona_facial_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_FEET = "persona_feet";
    static constexpr std::string_view PIECE_TYPE_PERSONA_HAIR = "persona_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_MOUTH = "persona_mouth";
    static constexpr std::string_view PIECE_TYPE_PERSONA_SKELETON = "persona_skeleton";
    static constexpr std::string_view PIECE_TYPE_PERSONA_SKIN = "persona_skin";
    static constexpr std::string_view PIECE_TYPE_PERSONA_TOP = "persona_top";
    std::string pieceId;
    std::string pieceType;
    std::string packId;
    bool isDefaultPiece;
    std::string productId;

    PersonaSkinPiece() = default;
    explicit PersonaSkinPiece(
        const std::string& pieceIdValue,
        const std::string& pieceTypeValue,
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
        pieceId = stream.readString().value_or("");
        pieceType = stream.readString().value_or("");
        packId = stream.readString().value_or("");
        isDefaultPiece = stream.readBool().value_or(false);
        productId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(pieceId);
        stream.writeString(pieceType);
        stream.writeString(packId);
        stream.writeBool(isDefaultPiece);
        stream.writeString(productId);
    }
};
