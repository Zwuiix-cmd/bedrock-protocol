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
    static constexpr std::string_view PIECE_TYPE_PERSONA_EYES = "persona_eyes";
    static constexpr std::string_view PIECE_TYPE_PERSONA_HAIR = "persona_hair";
    static constexpr std::string_view PIECE_TYPE_PERSONA_MOUTH = "persona_mouth";
    std::string pieceType;
    std::vector<std::string> colors;

    PersonaPieceTintColor() = default;
    explicit PersonaPieceTintColor(const std::string& pieceTypeValue, const std::vector<std::string>& colorsValue) : pieceType(pieceTypeValue), colors(colorsValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        pieceType = stream.readString().value_or("");
        colors = stream.readList([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(pieceType);
        stream.writeList(colors, [&](const auto& item) {
            stream.writeString(item);
        });
    }
};
