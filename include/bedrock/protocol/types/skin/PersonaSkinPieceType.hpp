#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>
#include <string_view>

namespace bedrock::protocol {

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

} // namespace bedrock::protocol
namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::PersonaSkinPieceType>(::bedrock::protocol::PersonaSkinPieceType value) noexcept {
    switch (value) {
        case ::bedrock::protocol::PersonaSkinPieceType::Unknown: return "unknown";
        case ::bedrock::protocol::PersonaSkinPieceType::Skeleton: return "skeleton";
        case ::bedrock::protocol::PersonaSkinPieceType::Body: return "body";
        case ::bedrock::protocol::PersonaSkinPieceType::Skin: return "skin";
        case ::bedrock::protocol::PersonaSkinPieceType::Bottom: return "bottom";
        case ::bedrock::protocol::PersonaSkinPieceType::Feet: return "feet";
        case ::bedrock::protocol::PersonaSkinPieceType::Dress: return "dress";
        case ::bedrock::protocol::PersonaSkinPieceType::Top: return "top";
        case ::bedrock::protocol::PersonaSkinPieceType::HighPants: return "high_pants";
        case ::bedrock::protocol::PersonaSkinPieceType::Hands: return "hands";
        case ::bedrock::protocol::PersonaSkinPieceType::Outerwear: return "outerwear";
        case ::bedrock::protocol::PersonaSkinPieceType::FacialHair: return "facialhair";
        case ::bedrock::protocol::PersonaSkinPieceType::Mouth: return "mouth";
        case ::bedrock::protocol::PersonaSkinPieceType::Eyes: return "eyes";
        case ::bedrock::protocol::PersonaSkinPieceType::Hair: return "hair";
        case ::bedrock::protocol::PersonaSkinPieceType::Hood: return "hood";
        case ::bedrock::protocol::PersonaSkinPieceType::Back: return "back";
        case ::bedrock::protocol::PersonaSkinPieceType::FaceAccessory: return "faceaccessory";
        case ::bedrock::protocol::PersonaSkinPieceType::Head: return "head";
        case ::bedrock::protocol::PersonaSkinPieceType::Legs: return "legs";
        case ::bedrock::protocol::PersonaSkinPieceType::LeftLeg: return "leftleg";
        case ::bedrock::protocol::PersonaSkinPieceType::RightLeg: return "rightleg";
        case ::bedrock::protocol::PersonaSkinPieceType::Arms: return "arms";
        case ::bedrock::protocol::PersonaSkinPieceType::LeftArm: return "leftarm";
        case ::bedrock::protocol::PersonaSkinPieceType::RightArm: return "rightarm";
        case ::bedrock::protocol::PersonaSkinPieceType::Capes: return "capes";
        case ::bedrock::protocol::PersonaSkinPieceType::ClassicSkin: return "classicskin";
        case ::bedrock::protocol::PersonaSkinPieceType::Emote: return "emote";
        case ::bedrock::protocol::PersonaSkinPieceType::Coco: return "coco";
        case ::bedrock::protocol::PersonaSkinPieceType::Unsupported: return "unsupported";
        default: return invalid_tag;
    }
}

}
