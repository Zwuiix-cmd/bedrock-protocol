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
