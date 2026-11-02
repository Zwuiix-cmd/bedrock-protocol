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
#include <bedrock/protocol/types/skin/SkinArmSizeType.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>

class SkinData {
public:
    enum class TrustedSkinFlag : uint8_t {
        Unset = 0,
        False = 1,
        True = 2,
    };

    uint32_t skinColor;
    std::shared_ptr<SkinImage> capeImage;
    std::string fullSkinId;
    std::string skinId;
    std::string resourcePatch;
    std::shared_ptr<SkinImage> skinImage;
    std::vector<std::shared_ptr<SkinAnimation>> animations;
    std::string geometryDataJson = "{}";
    std::string geometryDataEngineVersion = "1.26.60";
    std::string animationData;
    std::string capeId;
    SkinArmSizeType armSize = SkinArmSizeType::Slim;
    std::vector<std::shared_ptr<PersonaSkinPiece>> personaPieces;
    std::vector<std::shared_ptr<PersonaPieceTintColor>> pieceTintColors;
    TrustedSkinFlag trustedSkinFlag = TrustedSkinFlag::Unset;
    bool premium = false;
    bool persona = false;
    bool personaCapeOnClassic = false;
    bool isPrimaryUser = true;
    bool override = true;
    std::string profileHash;

    SkinData() = default;
    explicit SkinData(
        const std::string& skinIdValue,
        const std::string& resourcePatchValue,
        const std::shared_ptr<SkinImage>& skinImageValue,
        const std::vector<std::shared_ptr<SkinAnimation>>& animationsValue,
        const std::shared_ptr<SkinImage>& capeImageValue,
        const std::string& geometryDataJsonValue,
        const std::string& geometryDataEngineVersionValue,
        const std::string& animationDataValue,
        const std::string& capeIdValue,
        const std::string& fullSkinIdValue,
        const SkinArmSizeType& armSizeValue,
        const uint32_t& skinColorValue,
        const std::vector<std::shared_ptr<PersonaSkinPiece>>& personaPiecesValue,
        const std::vector<std::shared_ptr<PersonaPieceTintColor>>& pieceTintColorsValue,
        const TrustedSkinFlag& trustedSkinFlagValue,
        const bool& premiumValue,
        const bool& personaValue,
        const bool& personaCapeOnClassicValue,
        const bool& isPrimaryUserValue,
        const bool& overrideValue,
        const std::string& profileHashValue
    ) :
        skinId(skinIdValue),
        resourcePatch(resourcePatchValue),
        skinImage(skinImageValue),
        animations(animationsValue),
        capeImage(capeImageValue),
        geometryDataJson(geometryDataJsonValue),
        geometryDataEngineVersion(geometryDataEngineVersionValue),
        animationData(animationDataValue),
        capeId(capeIdValue),
        fullSkinId(fullSkinIdValue),
        armSize(armSizeValue),
        skinColor(skinColorValue),
        personaPieces(personaPiecesValue),
        pieceTintColors(pieceTintColorsValue),
        trustedSkinFlag(trustedSkinFlagValue),
        premium(premiumValue),
        persona(personaValue),
        personaCapeOnClassic(personaCapeOnClassicValue),
        isPrimaryUser(isPrimaryUserValue),
        override(overrideValue),
        profileHash(profileHashValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        skinId = stream.readString().value_or("");
        resourcePatch = stream.readString().value_or("");
        geometryDataJson = stream.readString().value_or("");
        geometryDataEngineVersion = stream.readString().value_or("");
        animationData = stream.readString().value_or("");
        capeId = stream.readString().value_or("");
        fullSkinId = stream.readString().value_or("");
        const auto trustedSkinFlagRaw = stream.readString().value_or("");
        trustedSkinFlag = magic_enum::enum_cast<TrustedSkinFlag>(
            std::string_view(trustedSkinFlagRaw)
        ).value_or(TrustedSkinFlag::Unset);
        premium = stream.readBool().value_or(false);
        persona = stream.readBool().value_or(false);
        personaCapeOnClassic = stream.readBool().value_or(false);
        isPrimaryUser = stream.readBool().value_or(false);
        override = stream.readBool().value_or(false);
        profileHash = stream.readString().value_or("");
        skinColor = stream.readUnsignedInt().value_or(0);
        capeImage = std::make_shared<SkinImage>();
        capeImage->read(stream);
        skinImage = std::make_shared<SkinImage>();
        skinImage->read(stream);
        animations = stream.readObjectList<SkinAnimation>();
        armSize = magic_enum::enum_cast<SkinArmSizeType>(
            stream.readVarInt().value_or(0)
        ).value_or(SkinArmSizeType::Slim);
        personaPieces = stream.readObjectList<PersonaSkinPiece>();
        pieceTintColors = stream.readObjectList<PersonaPieceTintColor>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(skinId);
        stream.writeString(resourcePatch);
        stream.writeString(geometryDataJson);
        stream.writeString(geometryDataEngineVersion);
        stream.writeString(animationData);
        stream.writeString(capeId);
        stream.writeString(fullSkinId);
        stream.writeString(magic_enum::enum_name(trustedSkinFlag));
        stream.writeBool(premium);
        stream.writeBool(persona);
        stream.writeBool(personaCapeOnClassic);
        stream.writeBool(isPrimaryUser);
        stream.writeBool(override);
        stream.writeString(profileHash);
        stream.writeUnsignedInt(skinColor);
        capeImage->write(stream);
        skinImage->write(stream);
        stream.writeList(animations, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeVarInt(magic_enum::enum_integer(armSize));
        stream.writeList(personaPieces, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(pieceTintColors, [&](const auto& item) {
            item->write(stream);
        });
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<SkinData::TrustedSkinFlag>(SkinData::TrustedSkinFlag value) noexcept {
    switch (value) {
        case SkinData::TrustedSkinFlag::Unset: return "unset";
        case SkinData::TrustedSkinFlag::False: return "false";
        case SkinData::TrustedSkinFlag::True: return "true";
        default: return invalid_tag;
    }
}

}
