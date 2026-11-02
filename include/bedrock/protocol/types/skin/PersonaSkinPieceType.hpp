#pragma once

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
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<PersonaSkinPieceType>(PersonaSkinPieceType value) noexcept {
    switch (value) {
        case PersonaSkinPieceType::Unknown: return "unknown";
        case PersonaSkinPieceType::Skeleton: return "skeleton";
        case PersonaSkinPieceType::Body: return "body";
        case PersonaSkinPieceType::Skin: return "skin";
        case PersonaSkinPieceType::Bottom: return "bottom";
        case PersonaSkinPieceType::Feet: return "feet";
        case PersonaSkinPieceType::Dress: return "dress";
        case PersonaSkinPieceType::Top: return "top";
        case PersonaSkinPieceType::HighPants: return "high_pants";
        case PersonaSkinPieceType::Hands: return "hands";
        case PersonaSkinPieceType::Outerwear: return "outerwear";
        case PersonaSkinPieceType::FacialHair: return "facialhair";
        case PersonaSkinPieceType::Mouth: return "mouth";
        case PersonaSkinPieceType::Eyes: return "eyes";
        case PersonaSkinPieceType::Hair: return "hair";
        case PersonaSkinPieceType::Hood: return "hood";
        case PersonaSkinPieceType::Back: return "back";
        case PersonaSkinPieceType::FaceAccessory: return "faceaccessory";
        case PersonaSkinPieceType::Head: return "head";
        case PersonaSkinPieceType::Legs: return "legs";
        case PersonaSkinPieceType::LeftLeg: return "leftleg";
        case PersonaSkinPieceType::RightLeg: return "rightleg";
        case PersonaSkinPieceType::Arms: return "arms";
        case PersonaSkinPieceType::LeftArm: return "leftarm";
        case PersonaSkinPieceType::RightArm: return "rightarm";
        case PersonaSkinPieceType::Capes: return "capes";
        case PersonaSkinPieceType::ClassicSkin: return "classicskin";
        case PersonaSkinPieceType::Emote: return "emote";
        case PersonaSkinPieceType::Coco: return "coco";
        case PersonaSkinPieceType::Unsupported: return "unsupported";
        default: return invalid_tag;
    }
}

}
