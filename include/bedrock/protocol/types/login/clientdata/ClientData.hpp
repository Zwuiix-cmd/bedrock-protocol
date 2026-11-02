#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/login/clientdata/ClientDataAnimationFrame.hpp>
#include <bedrock/protocol/types/login/clientdata/ClientDataPersonaPieceTintColor.hpp>
#include <bedrock/protocol/types/login/clientdata/ClientDataPersonaSkinPiece.hpp>

class ClientData {
public:
    std::vector<std::shared_ptr<ClientDataAnimationFrame>> AnimatedImageData;
    std::string ArmSize;
    std::string CapeData;
    std::string CapeId;
    int32_t CapeImageHeight;
    int32_t CapeImageWidth;
    bool CapeOnClassicSkin;
    int32_t ClientRandomId;
    bool CompatibleWithClientSideChunkGen;
    int32_t CurrentInputMode;
    int32_t DefaultInputMode;
    std::string DeviceId;
    std::string DeviceModel;
    int32_t DeviceOS;
    std::string GameVersion;
    int32_t GraphicsMode;
    int32_t GuiScale;
    bool IsEditorMode;
    std::string LanguageCode;
    int32_t MaxViewDistance;
    int32_t MemoryTier;
    bool OverrideSkin;
    std::string PartyId;
    std::vector<std::shared_ptr<ClientDataPersonaSkinPiece>> PersonaPieces;
    bool PersonaSkin;
    std::vector<std::shared_ptr<ClientDataPersonaPieceTintColor>> PieceTintColors;
    std::string PlatformOfflineId;
    std::string PlatformOnlineId;
    int32_t PlatformType;
    std::string PlatformUserId;
    bool PremiumSkin = false;
    std::string SelfSignedId;
    std::string ServerAddress;
    std::string SkinAnimationData;
    std::string SkinColor;
    std::string SkinData;
    std::string SkinGeometryData;
    std::string SkinGeometryDataEngineVersion;
    std::string SkinId;
    int32_t SkinImageHeight;
    int32_t SkinImageWidth;
    std::string SkinResourcePatch;
    std::string ThirdPartyName;
    bool TrustedSkin;
    int32_t UIProfile;

    ClientData() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        AnimatedImageData = stream.readObjectList<ClientDataAnimationFrame>();
        ArmSize = stream.readString().value_or("");
        CapeData = stream.readString().value_or("");
        CapeId = stream.readString().value_or("");
        CapeImageHeight = stream.readInt().value_or(0);
        CapeImageWidth = stream.readInt().value_or(0);
        CapeOnClassicSkin = stream.readBool().value_or(false);
        ClientRandomId = stream.readInt().value_or(0);
        CompatibleWithClientSideChunkGen = stream.readBool().value_or(false);
        CurrentInputMode = stream.readInt().value_or(0);
        DefaultInputMode = stream.readInt().value_or(0);
        DeviceId = stream.readString().value_or("");
        DeviceModel = stream.readString().value_or("");
        DeviceOS = stream.readInt().value_or(0);
        GameVersion = stream.readString().value_or("");
        GraphicsMode = stream.readInt().value_or(0);
        GuiScale = stream.readInt().value_or(0);
        IsEditorMode = stream.readBool().value_or(false);
        LanguageCode = stream.readString().value_or("");
        MaxViewDistance = stream.readInt().value_or(0);
        MemoryTier = stream.readInt().value_or(0);
        OverrideSkin = stream.readBool().value_or(false);
        PartyId = stream.readString().value_or("");
        PersonaPieces = stream.readObjectList<ClientDataPersonaSkinPiece>();
        PersonaSkin = stream.readBool().value_or(false);
        PieceTintColors = stream.readObjectList<ClientDataPersonaPieceTintColor>();
        PlatformOfflineId = stream.readString().value_or("");
        PlatformOnlineId = stream.readString().value_or("");
        PlatformType = stream.readInt().value_or(0);
        PlatformUserId = stream.readString().value_or("");
        PremiumSkin = stream.readBool().value_or(false);
        SelfSignedId = stream.readString().value_or("");
        ServerAddress = stream.readString().value_or("");
        SkinAnimationData = stream.readString().value_or("");
        SkinColor = stream.readString().value_or("");
        SkinData = stream.readString().value_or("");
        SkinGeometryData = stream.readString().value_or("");
        SkinGeometryDataEngineVersion = stream.readString().value_or("");
        SkinId = stream.readString().value_or("");
        SkinImageHeight = stream.readInt().value_or(0);
        SkinImageWidth = stream.readInt().value_or(0);
        SkinResourcePatch = stream.readString().value_or("");
        ThirdPartyName = stream.readString().value_or("");
        TrustedSkin = stream.readBool().value_or(false);
        UIProfile = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(AnimatedImageData, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeString(ArmSize);
        stream.writeString(CapeData);
        stream.writeString(CapeId);
        stream.writeInt(CapeImageHeight);
        stream.writeInt(CapeImageWidth);
        stream.writeBool(CapeOnClassicSkin);
        stream.writeInt(ClientRandomId);
        stream.writeBool(CompatibleWithClientSideChunkGen);
        stream.writeInt(CurrentInputMode);
        stream.writeInt(DefaultInputMode);
        stream.writeString(DeviceId);
        stream.writeString(DeviceModel);
        stream.writeInt(DeviceOS);
        stream.writeString(GameVersion);
        stream.writeInt(GraphicsMode);
        stream.writeInt(GuiScale);
        stream.writeBool(IsEditorMode);
        stream.writeString(LanguageCode);
        stream.writeInt(MaxViewDistance);
        stream.writeInt(MemoryTier);
        stream.writeBool(OverrideSkin);
        stream.writeString(PartyId);
        stream.writeList(PersonaPieces, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeBool(PersonaSkin);
        stream.writeList(PieceTintColors, [&](const auto& item) {
            item->write(stream);
        });
        stream.writeString(PlatformOfflineId);
        stream.writeString(PlatformOnlineId);
        stream.writeInt(PlatformType);
        stream.writeString(PlatformUserId);
        stream.writeBool(PremiumSkin);
        stream.writeString(SelfSignedId);
        stream.writeString(ServerAddress);
        stream.writeString(SkinAnimationData);
        stream.writeString(SkinColor);
        stream.writeString(SkinData);
        stream.writeString(SkinGeometryData);
        stream.writeString(SkinGeometryDataEngineVersion);
        stream.writeString(SkinId);
        stream.writeInt(SkinImageHeight);
        stream.writeInt(SkinImageWidth);
        stream.writeString(SkinResourcePatch);
        stream.writeString(ThirdPartyName);
        stream.writeBool(TrustedSkin);
        stream.writeInt(UIProfile);
    }
};
