#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>

class SkinData {
public:
    enum class ArmSize : uint8_t {
        Slim = 0,
        Wide = 1,
    };

    std::shared_ptr<SkinImage> capeImage;
    std::string fullSkinId;
    std::string skinId;
    std::string playFabId;
    std::string resourcePatch;
    std::shared_ptr<SkinImage> skinImage;
    std::vector<std::shared_ptr<SkinAnimation>> animations;
    std::string geometryData;
    std::string geometryDataEngineVersion = "1.26.10";
    std::string animationData;
    std::string capeId;
    ArmSize armSize = ArmSize::Slim;
    std::string skinColor;
    std::vector<std::shared_ptr<PersonaSkinPiece>> personaPieces;
    std::vector<std::shared_ptr<PersonaPieceTintColor>> pieceTintColors;
    bool isVerified = true;
    bool premium = false;
    bool persona = false;
    bool personaCapeOnClassic = false;
    bool isPrimaryUser = true;
    bool override = true;

    SkinData() = default;
    explicit SkinData(
        const std::string& skinIdValue,
        const std::string& playFabIdValue,
        const std::string& resourcePatchValue,
        const std::shared_ptr<SkinImage>& skinImageValue,
        const std::vector<std::shared_ptr<SkinAnimation>>& animationsValue,
        const std::shared_ptr<SkinImage>& capeImageValue,
        const std::string& geometryDataValue,
        const std::string& geometryDataEngineVersionValue,
        const std::string& animationDataValue,
        const std::string& capeIdValue,
        const std::string& fullSkinIdValue,
        const ArmSize& armSizeValue,
        const std::string& skinColorValue,
        const std::vector<std::shared_ptr<PersonaSkinPiece>>& personaPiecesValue,
        const std::vector<std::shared_ptr<PersonaPieceTintColor>>& pieceTintColorsValue,
        const bool& isVerifiedValue,
        const bool& premiumValue,
        const bool& personaValue,
        const bool& personaCapeOnClassicValue,
        const bool& isPrimaryUserValue,
        const bool& overrideValue
    ) :
        skinId(skinIdValue),
        playFabId(playFabIdValue),
        resourcePatch(resourcePatchValue),
        skinImage(skinImageValue),
        animations(animationsValue),
        capeImage(capeImageValue),
        geometryData(geometryDataValue),
        geometryDataEngineVersion(geometryDataEngineVersionValue),
        animationData(animationDataValue),
        capeId(capeIdValue),
        fullSkinId(fullSkinIdValue),
        armSize(armSizeValue),
        skinColor(skinColorValue),
        personaPieces(personaPiecesValue),
        pieceTintColors(pieceTintColorsValue),
        isVerified(isVerifiedValue),
        premium(premiumValue),
        persona(personaValue),
        personaCapeOnClassic(personaCapeOnClassicValue),
        isPrimaryUser(isPrimaryUserValue),
        override(overrideValue) {

    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        skinId = stream.readString().value_or("");
        playFabId = stream.readString().value_or("");
        resourcePatch = stream.readString().value_or("");
        skinImage = std::make_shared<SkinImage>();
        skinImage->read(stream);
        animations = stream.readObjectList<SkinAnimation>();
        capeImage = std::make_shared<SkinImage>();
        capeImage->read(stream);
        geometryData = stream.readString().value_or("");
        geometryDataEngineVersion = stream.readString().value_or("");
        animationData = stream.readString().value_or("");
        capeId = stream.readString().value_or("");
        fullSkinId = stream.readString().value_or("");
        const auto armSizeRaw = stream.readString().value_or("");
        armSize = magic_enum::enum_cast<ArmSize>(
            std::string_view(armSizeRaw)
        ).value_or(ArmSize::Slim);
        skinColor = stream.readString().value_or("");
        personaPieces = stream.readObjectList<PersonaSkinPiece>();
        pieceTintColors = stream.readObjectList<PersonaPieceTintColor>();
        isVerified = stream.readBool().value_or(false);
        premium = stream.readBool().value_or(false);
        persona = stream.readBool().value_or(false);
        personaCapeOnClassic = stream.readBool().value_or(false);
        isPrimaryUser = stream.readBool().value_or(false);
        override = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(skinId);
        stream.writeString(playFabId);
        stream.writeString(resourcePatch);
        skinImage->write(stream);
        stream.writeList(animations, [&](const auto& item) {
            item->write(stream);
        });
        capeImage->write(stream);
        stream.writeString(geometryData);
        stream.writeString(geometryDataEngineVersion);
        stream.writeString(animationData);
        stream.writeString(capeId);
        stream.writeString(fullSkinId);
        stream.writeString(magic_enum::enum_name(armSize));
        stream.writeString(skinColor);
        stream.writeList(personaPieces, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeList(pieceTintColors, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeBool(isVerified);
        stream.writeBool(premium);
        stream.writeBool(persona);
        stream.writeBool(personaCapeOnClassic);
        stream.writeBool(isPrimaryUser);
        stream.writeBool(override);
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<SkinData::ArmSize>(SkinData::ArmSize value) noexcept {
    switch (value) {
        case SkinData::ArmSize::Slim: return "slim";
        case SkinData::ArmSize::Wide: return "wide";
        default: return invalid_tag;
    }
}

}
