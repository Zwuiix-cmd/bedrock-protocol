#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>


class ClientDataPersonaSkinPiece {
public:
    std::string PieceId;
    std::string PieceType;
    std::string PackId;
    bool IsDefault;
    std::string ProductId;

    ClientDataPersonaSkinPiece() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        PieceId = stream.readString().value_or("");
        PieceType = stream.readString().value_or("");
        PackId = stream.readString().value_or("");
        IsDefault = stream.readBool().value_or(false);
        ProductId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(PieceId);
        stream.writeString(PieceType);
        stream.writeString(PackId);
        stream.writeBool(IsDefault);
        stream.writeString(ProductId);
    }
};
