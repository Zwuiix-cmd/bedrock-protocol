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
#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/PersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/skin/PersonaSkinPiece.hpp>
#include <bedrock/protocol/types/skin/SkinAnimation.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>

class SkinData {
public:
    static constexpr std::string_view ARM_SIZE_SLIM = "slim";
    static constexpr std::string_view ARM_SIZE_WIDE = "wide";
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
    std::string armSize = "wide";
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
        const std::string& armSizeValue,
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
        geometryData = stream.readString().value_or("");
        geometryDataEngineVersion = stream.readString().value_or("");
        animationData = stream.readString().value_or("");
        capeId = stream.readString().value_or("");
        fullSkinId = stream.readString().value_or("");
        armSize = stream.readString().value_or("");
        skinColor = stream.readString().value_or("");
        premium = stream.readBool().value_or(false);
        persona = stream.readBool().value_or(false);
        personaCapeOnClassic = stream.readBool().value_or(false);
        isPrimaryUser = stream.readBool().value_or(false);
        override = stream.readBool().value_or(false);
        capeImage = ([&]() {
    auto value = std::make_shared<SkinImage>();
    value->read(stream);
    return value;
}());
        skinImage = ([&]() {
    auto value = std::make_shared<SkinImage>();
    value->read(stream);
    return value;
}());
        animations = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<SkinAnimation>();
            value->read(stream);
            return value;
            }());
        });
        personaPieces = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<PersonaSkinPiece>();
            value->read(stream);
            return value;
            }());
        });
        pieceTintColors = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<PersonaPieceTintColor>();
            value->read(stream);
            return value;
            }());
        });
        isVerified = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(skinId);
        stream.writeString(playFabId);
        stream.writeString(resourcePatch);
        stream.writeString(geometryData);
        stream.writeString(geometryDataEngineVersion);
        stream.writeString(animationData);
        stream.writeString(capeId);
        stream.writeString(fullSkinId);
        stream.writeString(armSize);
        stream.writeString(skinColor);
        stream.writeBool(premium);
        stream.writeBool(persona);
        stream.writeBool(personaCapeOnClassic);
        stream.writeBool(isPrimaryUser);
        stream.writeBool(override);
        if (capeImage) capeImage->write(stream);
        if (skinImage) skinImage->write(stream);
        stream.writeList(animations, [&](const auto& item) {
            if (item) item->write(stream);
        });
        stream.writeList(personaPieces, [&](const auto& item) {
            if (item) item->write(stream);
        });
        stream.writeList(pieceTintColors, [&](const auto& item) {
            if (item) item->write(stream);
        });
        stream.writeBool(isVerified);
    }
};
