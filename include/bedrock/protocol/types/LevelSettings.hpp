#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <utility>
#include <map>

#include <bedrock/protocol/types/ChatRestrictionLevel.hpp>
#include <bedrock/protocol/types/EditorWorldType.hpp>
#include <bedrock/protocol/types/EducationUriResource.hpp>
#include <bedrock/protocol/types/Experiments.hpp>
#include <bedrock/protocol/types/GameRule.hpp>
#include <bedrock/protocol/types/SpawnSettings.hpp>

class LevelSettings {
public:
    uint64_t seed;
    std::shared_ptr<SpawnSettings> spawnSettings;
    int32_t generator = 1;
    int32_t worldGamemode;
    bool hardcore = false;
    int32_t difficulty;
    glm::ivec3 spawnPosition;
    bool hasAchievementsDisabled = true;
    EditorWorldType editorWorldType = EditorWorldType::NonEditor;
    bool createdInEditorMode = false;
    bool exportedFromEditorMode = false;
    int32_t time = -1;
    int32_t eduEditionOffer = 0;
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
    int32_t defaultPlayerPermission = 1;
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
    std::string vanillaVersion = "1.26.10";
    int32_t limitedWorldWidth = 0;
    int32_t limitedWorldLength = 0;
    bool isNewNether = true;
    std::optional<std::shared_ptr<EducationUriResource>> eduSharedUriResource;
    std::optional<bool> experimentalGameplayOverride;
    ChatRestrictionLevel chatRestrictionLevel = ChatRestrictionLevel::None;
    bool disablePlayerInteractions = false;

    LevelSettings() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        seed = stream.readUnsignedLong().value_or(0);
        generator = stream.readVarInt().value_or(0);
        worldGamemode = stream.readVarInt().value_or(0);
        hardcore = stream.readBool().value_or(false);
        difficulty = stream.readVarInt().value_or(0);
        spawnPosition = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        hasAchievementsDisabled = stream.readBool().value_or(false);
        editorWorldType = magic_enum::enum_cast<EditorWorldType>(
            stream.readVarInt().value_or(0)
        ).value_or(EditorWorldType::NonEditor);
        createdInEditorMode = stream.readBool().value_or(false);
        exportedFromEditorMode = stream.readBool().value_or(false);
        time = stream.readVarInt().value_or(0);
        eduEditionOffer = stream.readVarInt().value_or(0);
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
        defaultPlayerPermission = stream.readVarInt().value_or(0);
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
        experimentalGameplayOverride = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        chatRestrictionLevel = magic_enum::enum_cast<ChatRestrictionLevel>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(ChatRestrictionLevel::None);
        disablePlayerInteractions = stream.readBool().value_or(false);
        spawnSettings = std::make_shared<SpawnSettings>();
        spawnSettings->read(stream);
        const auto gameRulesCount = stream.readUnsignedInt().value_or(0);
        for (std::size_t index = 0; index < gameRulesCount; ++index) {
            auto gameRulesKey = stream.readString().value_or("");
            auto gameRulesValue = GameRule::create(stream);
            gameRules.emplace(std::move(gameRulesKey), std::move(gameRulesValue));
        }
        experiments = std::make_shared<Experiments>();
        experiments->read(stream);
        eduSharedUriResource = stream.readOptional([&]() {
            return stream.readObject<EducationUriResource>();
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedLong(seed);
		spawnSettings->write(stream);
		stream.writeVarInt(generator);
		stream.writeVarInt(worldGamemode);
		stream.writeBool(hardcore);
		stream.writeVarInt(difficulty);
		stream.writeSignedBlockPos(spawnPosition);
		stream.writeBool(hasAchievementsDisabled);
		stream.writeVarInt(magic_enum::enum_integer(editorWorldType));
		stream.writeBool(createdInEditorMode);
		stream.writeBool(exportedFromEditorMode);
		stream.writeVarInt(time);
		stream.writeVarInt(eduEditionOffer);
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
		GameRule::writeRules(stream, gameRules, true);
		experiments->write(stream);
		stream.writeBool(hasBonusChestEnabled);
		stream.writeBool(hasStartWithMapEnabled);
		stream.writeVarInt(defaultPlayerPermission);
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
		eduSharedUriResource.value_or(std::make_shared<EducationUriResource>("", ""))->write(stream);
		stream.writeOptional(experimentalGameplayOverride, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeUnsignedByte(magic_enum::enum_integer(chatRestrictionLevel));
		stream.writeBool(disablePlayerInteractions);
    }
};
