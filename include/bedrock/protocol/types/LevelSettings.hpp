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
#include <map>
#include <bedrock/protocol/types/ChatRestrictionLevel.hpp>
#include <bedrock/protocol/types/EditorLevelMigrationVersion.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationUriResource.hpp>
#include <bedrock/protocol/types/Experiments.hpp>
#include <bedrock/protocol/types/GameMode.hpp>
#include <bedrock/protocol/types/GameRule.hpp>
#include <bedrock/protocol/types/PlayerPermissions.hpp>
#include <bedrock/protocol/types/SpawnSettings.hpp>
#include <bedrock/protocol/types/ChatRestrictionLevel.hpp>
#include <bedrock/protocol/types/EditorLevelMigrationVersion.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationUriResource.hpp>
#include <bedrock/protocol/types/Experiments.hpp>
#include <bedrock/protocol/types/GameRule.hpp>
#include <bedrock/protocol/types/SpawnSettings.hpp>
#include <bedrock/protocol/types/GameMode.hpp>
#include <bedrock/protocol/types/PlayerPermissions.hpp>
#include <bedrock/protocol/types/EducationUriResource.hpp>
#include <bedrock/protocol/types/Experiments.hpp>
#include <bedrock/protocol/types/SpawnSettings.hpp>
#include <bedrock/protocol/types/GameMode.hpp>
#include <bedrock/protocol/types/PlayerPermissions.hpp>

class LevelSettings {
public:
    uint64_t seed;
    std::shared_ptr<SpawnSettings> spawnSettings;
    int32_t generator = 1;
    GameMode worldGamemode;
    bool hardcore = false;
    int32_t difficulty;
    glm::ivec3 spawnPosition;
    bool hasAchievementsDisabled = true;
    EditorWorldType editorWorldType = EditorWorldType::NonEditor;
    bool createdInEditorMode = false;
    bool exportedFromEditorMode = false;
    EditorLevelMigrationVersion editorLevelMigrationVersion = EditorLevelMigrationVersion::Legacy;
    int32_t time = -1;
    uint32_t eduEditionOffer = 0;
    bool hasEduFeaturesEnabled = false;
    std::string eduProductUUID;
    float rainLevel;
    float lightningLevel;
    bool hasConfirmedPlatformLockedContent = false;
    bool isMultiplayerGame = true;
    bool hasLANBroadcast = true;
    int32_t xboxLiveBroadcastMode = 4;
    int32_t platformBroadcastMode = 4;
    bool commandsEnabled;
    bool isTexturePacksRequired = true;
    std::map<std::string, std::shared_ptr<GameRule>> gameRules;
    std::shared_ptr<Experiments> experiments;
    bool hasBonusChestEnabled = false;
    bool hasStartWithMapEnabled = false;
    PlayerPermissions defaultPlayerPermission = static_cast<PlayerPermissions>(0);
    int32_t serverChunkTickRadius = 4;
    bool hasLockedBehaviorPack = false;
    bool hasLockedResourcePack = false;
    bool isFromLockedWorldTemplate = false;
    bool useMsaGamertagsOnly = false;
    bool isFromWorldTemplate = false;
    bool isWorldTemplateOptionLocked = false;
    bool onlySpawnV1Villagers = false;
    bool disablePersona = false;
    bool disableCustomSkins = false;
    bool muteEmoteAnnouncements = false;
    std::string vanillaVersion = "1.26.60";
    int32_t limitedWorldWidth = 0;
    int32_t limitedWorldLength = 0;
    bool isNewNether = true;
    std::optional<std::shared_ptr<EducationUriResource>> eduSharedUriResource;
    std::optional<bool> experimentalGameplayOverride;
    ChatRestrictionLevel chatRestrictionLevel = ChatRestrictionLevel::None;
    bool disablePlayerInteractions = false;
    int32_t serverEditorConnectionPolicy = 0;
    bool allowAnonymousBlockDropsInEditorWorlds = false;

    LevelSettings() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        seed = stream.readUnsignedLong().value_or(0);
        generator = stream.readVarInt().value_or(0);
        hardcore = stream.readBool().value_or(false);
        difficulty = stream.readVarInt().value_or(0);
        spawnPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        hasAchievementsDisabled = stream.readBool().value_or(false);
        editorWorldType = magic_enum::enum_cast<EditorWorldType>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<EditorWorldType>(0));
        createdInEditorMode = stream.readBool().value_or(false);
        exportedFromEditorMode = stream.readBool().value_or(false);
        editorLevelMigrationVersion = magic_enum::enum_cast<EditorLevelMigrationVersion>(stream.readByte().value_or(0)).value_or(magic_enum::enum_value<EditorLevelMigrationVersion>(0));
        time = stream.readVarInt().value_or(0);
        eduEditionOffer = stream.readUnsignedVarInt().value_or(0);
        hasEduFeaturesEnabled = stream.readBool().value_or(false);
        eduProductUUID = stream.readString().value_or("");
        rainLevel = stream.readFloat().value_or(0.0f);
        lightningLevel = stream.readFloat().value_or(0.0f);
        hasConfirmedPlatformLockedContent = stream.readBool().value_or(false);
        isMultiplayerGame = stream.readBool().value_or(false);
        hasLANBroadcast = stream.readBool().value_or(false);
        xboxLiveBroadcastMode = stream.readVarInt().value_or(0);
        platformBroadcastMode = stream.readVarInt().value_or(0);
        commandsEnabled = stream.readBool().value_or(false);
        isTexturePacksRequired = stream.readBool().value_or(false);
        hasBonusChestEnabled = stream.readBool().value_or(false);
        hasStartWithMapEnabled = stream.readBool().value_or(false);
        serverChunkTickRadius = stream.readInt().value_or(0);
        hasLockedBehaviorPack = stream.readBool().value_or(false);
        hasLockedResourcePack = stream.readBool().value_or(false);
        isFromLockedWorldTemplate = stream.readBool().value_or(false);
        useMsaGamertagsOnly = stream.readBool().value_or(false);
        isFromWorldTemplate = stream.readBool().value_or(false);
        isWorldTemplateOptionLocked = stream.readBool().value_or(false);
        onlySpawnV1Villagers = stream.readBool().value_or(false);
        disablePersona = stream.readBool().value_or(false);
        disableCustomSkins = stream.readBool().value_or(false);
        muteEmoteAnnouncements = stream.readBool().value_or(false);
        vanillaVersion = stream.readString().value_or("");
        limitedWorldWidth = stream.readInt().value_or(0);
        limitedWorldLength = stream.readInt().value_or(0);
        isNewNether = stream.readBool().value_or(false);
        chatRestrictionLevel = magic_enum::enum_cast<ChatRestrictionLevel>(stream.readUnsignedByte().value_or(0)).value_or(magic_enum::enum_value<ChatRestrictionLevel>(0));
        disablePlayerInteractions = stream.readBool().value_or(false);
        serverEditorConnectionPolicy = stream.readVarInt().value_or(0);
        allowAnonymousBlockDropsInEditorWorlds = stream.readBool().value_or(false);
        spawnSettings = ([&]() {
    auto value = std::make_shared<SpawnSettings>();
    value->read(stream);
    return value;
}());
        worldGamemode = magic_enum::enum_cast<GameMode>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<GameMode>(0));
        gameRules = ([&]() {
    std::map<std::string, std::shared_ptr<GameRule>> result;
    const auto count = stream.readUnsignedInt().value_or(0);
    for (uint32_t i = 0; i < count; ++i) {
        auto key = stream.readString().value_or("");
        auto value = GameRule::decode(stream);
        result.emplace(std::move(key), std::move(value));
    }
    return result;
}());
        experiments = ([&]() {
    auto value = std::make_shared<Experiments>();
    value->read(stream);
    return value;
}());
        defaultPlayerPermission = magic_enum::enum_cast<PlayerPermissions>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<PlayerPermissions>(0));
        eduSharedUriResource = stream.readOptional([&]() {
	return ([&]() {
	auto value = std::make_shared<EducationUriResource>();
	value->read(stream);
	return value;
	}());
});
        experimentalGameplayOverride = stream.readOptional([&]() {
	return stream.readBool().value_or(false);
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedLong(seed);
        stream.writeVarInt(generator);
        stream.writeBool(hardcore);
        stream.writeVarInt(difficulty);
        stream.writeSignedBlockPos(spawnPosition);
        stream.writeBool(hasAchievementsDisabled);
        stream.writeVarInt(magic_enum::enum_integer(editorWorldType));
        stream.writeBool(createdInEditorMode);
        stream.writeBool(exportedFromEditorMode);
        stream.writeByte(magic_enum::enum_integer(editorLevelMigrationVersion));
        stream.writeVarInt(time);
        stream.writeUnsignedVarInt(eduEditionOffer);
        stream.writeBool(hasEduFeaturesEnabled);
        stream.writeString(eduProductUUID);
        stream.writeFloat(rainLevel);
        stream.writeFloat(lightningLevel);
        stream.writeBool(hasConfirmedPlatformLockedContent);
        stream.writeBool(isMultiplayerGame);
        stream.writeBool(hasLANBroadcast);
        stream.writeVarInt(xboxLiveBroadcastMode);
        stream.writeVarInt(platformBroadcastMode);
        stream.writeBool(commandsEnabled);
        stream.writeBool(isTexturePacksRequired);
        stream.writeBool(hasBonusChestEnabled);
        stream.writeBool(hasStartWithMapEnabled);
        stream.writeInt(serverChunkTickRadius);
        stream.writeBool(hasLockedBehaviorPack);
        stream.writeBool(hasLockedResourcePack);
        stream.writeBool(isFromLockedWorldTemplate);
        stream.writeBool(useMsaGamertagsOnly);
        stream.writeBool(isFromWorldTemplate);
        stream.writeBool(isWorldTemplateOptionLocked);
        stream.writeBool(onlySpawnV1Villagers);
        stream.writeBool(disablePersona);
        stream.writeBool(disableCustomSkins);
        stream.writeBool(muteEmoteAnnouncements);
        stream.writeString(vanillaVersion);
        stream.writeInt(limitedWorldWidth);
        stream.writeInt(limitedWorldLength);
        stream.writeBool(isNewNether);
        stream.writeUnsignedByte(magic_enum::enum_integer(chatRestrictionLevel));
        stream.writeBool(disablePlayerInteractions);
        stream.writeVarInt(serverEditorConnectionPolicy);
        stream.writeBool(allowAnonymousBlockDropsInEditorWorlds);
        if (spawnSettings) spawnSettings->write(stream);
        stream.writeVarInt(magic_enum::enum_integer(worldGamemode));
        stream.writeUnsignedInt(gameRules.size());
for (const auto& [key, item] : gameRules) {
    stream.writeString(key);
    if (item) item->write(stream);
}
        if (experiments) experiments->write(stream);
        stream.writeVarInt(magic_enum::enum_integer(defaultPlayerPermission));
        stream.writeOptional(eduSharedUriResource, [&](const auto& optionalValue) {
	if (optionalValue) optionalValue->write(stream);
});
        stream.writeOptional(experimentalGameplayOverride, [&](const auto& optionalValue) {
	stream.writeBool(optionalValue);
});
    }
};
