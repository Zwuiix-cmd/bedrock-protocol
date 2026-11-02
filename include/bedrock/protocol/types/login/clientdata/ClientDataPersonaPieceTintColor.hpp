#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>
#include <vector>


class ClientDataPersonaPieceTintColor {
public:
    std::string PieceType;
    std::vector<std::string> Colors;

    ClientDataPersonaPieceTintColor() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        PieceType = stream.readString().value_or("");
        Colors = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(PieceType);
        stream.writeList(Colors, [&](const auto& item) {
            stream.writeString(item);
        });
    }
};
