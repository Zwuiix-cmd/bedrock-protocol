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
