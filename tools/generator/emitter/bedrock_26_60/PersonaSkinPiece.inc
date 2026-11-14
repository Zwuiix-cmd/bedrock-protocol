<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceId = stream.readString().value_or("");
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(
            stream.readString().value_or("")
        ).value_or(PersonaSkinPieceType::Unknown);
        packId = stream.readUUID().value_or("");
        isDefaultPiece = stream.readBool().value_or(false);
        productId = stream.readString().value_or("");
    }
BEFORE,
<<<'AFTER'
    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceId = stream.readString().value_or("");
        pieceType = magic_enum::enum_cast<PersonaSkinPieceType>(stream.readUnsignedInt().value_or(0)).value_or(PersonaSkinPieceType::Unknown);
        packId = stream.readUUID().value_or("");
        isDefaultPiece = stream.readBool().value_or(false);
        productId = stream.readString().value_or("");
    }
AFTER],
    [<<<'BEFORE'
    virtual void write(BinaryStream& stream) const {
        stream.writeString(pieceId);
        stream.writeString(magic_enum::enum_integer(pieceType));
        stream.writeUUID(packId);
        stream.writeBool(isDefaultPiece);
        stream.writeString(productId);
    }
BEFORE,
<<<'AFTER'
    virtual void write(BinaryStream& stream) const {
        stream.writeString(pieceId);
        stream.writeUnsignedInt(magic_enum::enum_integer(pieceType));
        stream.writeUUID(packId);
        stream.writeBool(isDefaultPiece);
        stream.writeString(productId);
    }
AFTER],
];
