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
#include <bedrock/protocol/types/skin/SkinImage.hpp>
#include <bedrock/protocol/types/skin/SkinArmSizeType.hpp>
#include <bedrock/protocol/types/skin/SkinImage.hpp>

class SkinData {
public:
    static constexpr std::string_view GEOMETRY_DATA_NONE = "{}";
    static constexpr std::string_view TRUSTED_SKIN_UNSET = "unset";
    static constexpr std::string_view TRUSTED_SKIN_FALSE = "false";
    static constexpr std::string_view TRUSTED_SKIN_TRUE = "true";
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
    SkinArmSizeType armSize = static_cast<SkinArmSizeType>(0);
    std::vector<std::shared_ptr<PersonaSkinPiece>> personaPieces;
    std::vector<std::shared_ptr<PersonaPieceTintColor>> pieceTintColors;
    std::string trustedSkinFlag = "true";
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
        const std::string& trustedSkinFlagValue,
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
        trustedSkinFlag = stream.readString().value_or("");
        premium = stream.readBool().value_or(false);
        persona = stream.readBool().value_or(false);
        personaCapeOnClassic = stream.readBool().value_or(false);
        isPrimaryUser = stream.readBool().value_or(false);
        override = stream.readBool().value_or(false);
        profileHash = stream.readString().value_or("");
        skinColor = stream.readUnsignedInt().value_or(0);
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
        armSize = static_cast<SkinArmSizeType>(stream.readVarInt().value_or(0));
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
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(skinId);
        stream.writeString(resourcePatch);
        stream.writeString(geometryDataJson);
        stream.writeString(geometryDataEngineVersion);
        stream.writeString(animationData);
        stream.writeString(capeId);
        stream.writeString(fullSkinId);
        stream.writeString(trustedSkinFlag);
        stream.writeBool(premium);
        stream.writeBool(persona);
        stream.writeBool(personaCapeOnClassic);
        stream.writeBool(isPrimaryUser);
        stream.writeBool(override);
        stream.writeString(profileHash);
        stream.writeUnsignedInt(skinColor);
        if (capeImage) capeImage->write(stream);
        if (skinImage) skinImage->write(stream);
        stream.writeList(animations, [&](const auto& item) {
	if (item) item->write(stream);
});
        stream.writeVarInt(magic_enum::enum_integer(armSize));
        stream.writeList(personaPieces, [&](const auto& item) {
	if (item) item->write(stream);
});
        stream.writeList(pieceTintColors, [&](const auto& item) {
	if (item) item->write(stream);
});
    }
};
