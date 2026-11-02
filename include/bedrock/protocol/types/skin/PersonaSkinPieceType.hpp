#pragma once

#include <array>
#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

enum class PersonaSkinPieceType : uint16_t {
    Unknown = 0,
    Skeleton = 1,
    Body = 2,
    Skin = 3,
    Bottom = 4,
    Feet = 5,
    Dress = 6,
    Top = 7,
    HighPants = 8,
    Hands = 9,
    Outerwear = 10,
    FacialHair = 11,
    Mouth = 12,
    Eyes = 13,
    Hair = 14,
    Hood = 15,
    Back = 16,
    FaceAccessory = 17,
    Head = 18,
    Legs = 19,
    LeftLeg = 20,
    RightLeg = 21,
    Arms = 22,
    LeftArm = 23,
    RightArm = 24,
    Capes = 25,
    ClassicSkin = 26,
    Emote = 27,
    Coco = 28,
    Unsupported = 29,
};

inline constexpr std::array<std::string_view, 30> PersonaSkinPieceTypeNames = {
#include <bedrock/protocol/types/skin/PersonaSkinPieceTypeNames.inc>
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<PersonaSkinPieceType>(PersonaSkinPieceType value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < PersonaSkinPieceTypeNames.size() ? customize_t{PersonaSkinPieceTypeNames[index]} : invalid_tag;
}

}
