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
#include <bedrock/protocol/types/skin/PersonaSkinPieceType.hpp>

class PersonaSkinPiece {
public:
    std::string pieceId;
    PersonaSkinPieceType pieceType;
    std::string packId;
    bool isDefaultPiece;
    std::string productId;

    PersonaSkinPiece() = default;
    explicit PersonaSkinPiece(
        const std::string& pieceIdValue,
        const PersonaSkinPieceType& pieceTypeValue,
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
        isDefaultPiece = stream.readBool().value_or(false);
        productId = stream.readString().value_or("");
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(
            stream.readVarInt().value_or(0)
        ).value_or(PersonaSkinPieceType::Unknown);
        packId = stream.readUUID().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(pieceId);
        stream.writeBool(isDefaultPiece);
        stream.writeString(productId);
        stream.writeVarInt(magic_enum::enum_integer(pieceType));
        stream.writeUUID(packId);
    }
};
