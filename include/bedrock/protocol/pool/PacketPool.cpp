#include <bedrock/protocol/pool/PacketPool.hpp>

#include <bedrock/protocol/LoginPacket.hpp>
#include <bedrock/protocol/PlayStatusPacket.hpp>
#include <bedrock/protocol/ServerToClientHandshakePacket.hpp>
#include <bedrock/protocol/ClientToServerHandshakePacket.hpp>
#include <bedrock/protocol/DisconnectPacket.hpp>
#include <bedrock/protocol/ResourcePacksInfoPacket.hpp>
#include <bedrock/protocol/ResourcePackStackPacket.hpp>
#include <bedrock/protocol/ResourcePackClientResponsePacket.hpp>
#include <bedrock/protocol/TextPacket.hpp>
#include <bedrock/protocol/SetTimePacket.hpp>
#include <bedrock/protocol/StartGamePacket.hpp>
#include <bedrock/protocol/AddPlayerPacket.hpp>
#include <bedrock/protocol/AddActorPacket.hpp>
#include <bedrock/protocol/RemoveActorPacket.hpp>
#include <bedrock/protocol/AddItemActorPacket.hpp>
#include <bedrock/protocol/ServerPlayerPostMovePositionPacket.hpp>
#include <bedrock/protocol/TakeItemActorPacket.hpp>
#include <bedrock/protocol/MoveActorAbsolutePacket.hpp>
#include <bedrock/protocol/MovePlayerPacket.hpp>
#include <bedrock/protocol/UpdateBlockPacket.hpp>
#include <bedrock/protocol/AddPaintingPacket.hpp>
#include <bedrock/protocol/LevelEventPacket.hpp>
#include <bedrock/protocol/BlockEventPacket.hpp>
#include <bedrock/protocol/ActorEventPacket.hpp>
#include <bedrock/protocol/MobEffectPacket.hpp>
#include <bedrock/protocol/UpdateAttributesPacket.hpp>
#include <bedrock/protocol/InventoryTransactionPacket.hpp>
#include <bedrock/protocol/MobEquipmentPacket.hpp>
#include <bedrock/protocol/MobArmorEquipmentPacket.hpp>
#include <bedrock/protocol/InteractPacket.hpp>
#include <bedrock/protocol/BlockPickRequestPacket.hpp>
#include <bedrock/protocol/ActorPickRequestPacket.hpp>
#include <bedrock/protocol/PlayerActionPacket.hpp>
#include <bedrock/protocol/HurtArmorPacket.hpp>
#include <bedrock/protocol/SetActorDataPacket.hpp>
#include <bedrock/protocol/SetActorMotionPacket.hpp>
#include <bedrock/protocol/SetActorLinkPacket.hpp>
#include <bedrock/protocol/SetHealthPacket.hpp>
#include <bedrock/protocol/SetSpawnPositionPacket.hpp>
#include <bedrock/protocol/AnimatePacket.hpp>
#include <bedrock/protocol/RespawnPacket.hpp>
#include <bedrock/protocol/ContainerOpenPacket.hpp>
#include <bedrock/protocol/ContainerClosePacket.hpp>
#include <bedrock/protocol/PlayerHotbarPacket.hpp>
#include <bedrock/protocol/InventoryContentPacket.hpp>
#include <bedrock/protocol/InventorySlotPacket.hpp>
#include <bedrock/protocol/ContainerSetDataPacket.hpp>
#include <bedrock/protocol/CraftingDataPacket.hpp>
#include <bedrock/protocol/GuiDataPickItemPacket.hpp>
#include <bedrock/protocol/BlockActorDataPacket.hpp>
#include <bedrock/protocol/LevelChunkPacket.hpp>
#include <bedrock/protocol/SetCommandsEnabledPacket.hpp>
#include <bedrock/protocol/SetDifficultyPacket.hpp>
#include <bedrock/protocol/ChangeDimensionPacket.hpp>
#include <bedrock/protocol/SetPlayerGameTypePacket.hpp>
#include <bedrock/protocol/PlayerListPacket.hpp>
#include <bedrock/protocol/SimpleEventPacket.hpp>
#include <bedrock/protocol/LegacyTelemetryEventPacket.hpp>
#include <bedrock/protocol/SpawnExperienceOrbPacket.hpp>
#include <bedrock/protocol/ClientboundMapItemDataPacket.hpp>
#include <bedrock/protocol/MapInfoRequestPacket.hpp>
#include <bedrock/protocol/RequestChunkRadiusPacket.hpp>
#include <bedrock/protocol/ChunkRadiusUpdatedPacket.hpp>
#include <bedrock/protocol/GameRulesChangedPacket.hpp>
#include <bedrock/protocol/CameraPacket.hpp>
#include <bedrock/protocol/BossEventPacket.hpp>
#include <bedrock/protocol/ShowCreditsPacket.hpp>
#include <bedrock/protocol/AvailableCommandsPacket.hpp>
#include <bedrock/protocol/CommandRequestPacket.hpp>
#include <bedrock/protocol/CommandBlockUpdatePacket.hpp>
#include <bedrock/protocol/CommandOutputPacket.hpp>
#include <bedrock/protocol/UpdateTradePacket.hpp>
#include <bedrock/protocol/UpdateEquipPacket.hpp>
#include <bedrock/protocol/ResourcePackDataInfoPacket.hpp>
#include <bedrock/protocol/ResourcePackChunkDataPacket.hpp>
#include <bedrock/protocol/ResourcePackChunkRequestPacket.hpp>
#include <bedrock/protocol/TransferPacket.hpp>
#include <bedrock/protocol/PlaySoundPacket.hpp>
#include <bedrock/protocol/StopSoundPacket.hpp>
#include <bedrock/protocol/SetTitlePacket.hpp>
#include <bedrock/protocol/AddBehaviorTreePacket.hpp>
#include <bedrock/protocol/StructureBlockUpdatePacket.hpp>
#include <bedrock/protocol/ShowStoreOfferPacket.hpp>
#include <bedrock/protocol/PurchaseReceiptPacket.hpp>
#include <bedrock/protocol/PlayerSkinPacket.hpp>
#include <bedrock/protocol/SubClientLoginPacket.hpp>
#include <bedrock/protocol/AutomationClientConnectPacket.hpp>
#include <bedrock/protocol/SetLastHurtByPacket.hpp>
#include <bedrock/protocol/BookEditPacket.hpp>
#include <bedrock/protocol/NpcRequestPacket.hpp>
#include <bedrock/protocol/PhotoTransferPacket.hpp>
#include <bedrock/protocol/ModalFormRequestPacket.hpp>
#include <bedrock/protocol/ModalFormResponsePacket.hpp>
#include <bedrock/protocol/ServerSettingsRequestPacket.hpp>
#include <bedrock/protocol/ServerSettingsResponsePacket.hpp>
#include <bedrock/protocol/ShowProfilePacket.hpp>
#include <bedrock/protocol/SetDefaultGameTypePacket.hpp>
#include <bedrock/protocol/RemoveObjectivePacket.hpp>
#include <bedrock/protocol/SetDisplayObjectivePacket.hpp>
#include <bedrock/protocol/SetScorePacket.hpp>
#include <bedrock/protocol/LabTablePacket.hpp>
#include <bedrock/protocol/UpdateBlockSyncedPacket.hpp>
#include <bedrock/protocol/MoveActorDeltaPacket.hpp>
#include <bedrock/protocol/SetScoreboardIdentityPacket.hpp>
#include <bedrock/protocol/SetLocalPlayerAsInitializedPacket.hpp>
#include <bedrock/protocol/UpdateSoftEnumPacket.hpp>
#include <bedrock/protocol/NetworkStackLatencyPacket.hpp>
#include <bedrock/protocol/SpawnParticleEffectPacket.hpp>
#include <bedrock/protocol/AvailableActorIdentifiersPacket.hpp>
#include <bedrock/protocol/NetworkChunkPublisherUpdatePacket.hpp>
#include <bedrock/protocol/BiomeDefinitionListPacket.hpp>
#include <bedrock/protocol/LevelSoundEventPacket.hpp>
#include <bedrock/protocol/LevelEventGenericPacket.hpp>
#include <bedrock/protocol/LecternUpdatePacket.hpp>
#include <bedrock/protocol/ClientCacheStatusPacket.hpp>
#include <bedrock/protocol/OnScreenTextureAnimationPacket.hpp>
#include <bedrock/protocol/MapCreateLockedCopyPacket.hpp>
#include <bedrock/protocol/StructureTemplateDataRequestPacket.hpp>
#include <bedrock/protocol/StructureTemplateDataResponsePacket.hpp>
#include <bedrock/protocol/ClientCacheBlobStatusPacket.hpp>
#include <bedrock/protocol/ClientCacheMissResponsePacket.hpp>
#include <bedrock/protocol/EducationSettingsPacket.hpp>
#include <bedrock/protocol/EmotePacket.hpp>
#include <bedrock/protocol/MultiplayerSettingsPacket.hpp>
#include <bedrock/protocol/SettingsCommandPacket.hpp>
#include <bedrock/protocol/AnvilDamagePacket.hpp>
#include <bedrock/protocol/CompletedUsingItemPacket.hpp>
#include <bedrock/protocol/NetworkSettingsPacket.hpp>
#include <bedrock/protocol/PlayerAuthInputPacket.hpp>
#include <bedrock/protocol/CreativeContentPacket.hpp>
#include <bedrock/protocol/PlayerEnchantOptionsPacket.hpp>
#include <bedrock/protocol/ItemStackRequestPacket.hpp>
#include <bedrock/protocol/ItemStackResponsePacket.hpp>
#include <bedrock/protocol/PlayerArmorDamagePacket.hpp>
#include <bedrock/protocol/CodeBuilderPacket.hpp>
#include <bedrock/protocol/UpdatePlayerGameTypePacket.hpp>
#include <bedrock/protocol/EmoteListPacket.hpp>
#include <bedrock/protocol/PositionTrackingDBServerBroadcastPacket.hpp>
#include <bedrock/protocol/PositionTrackingDBClientRequestPacket.hpp>
#include <bedrock/protocol/DebugInfoPacket.hpp>
#include <bedrock/protocol/PacketViolationWarningPacket.hpp>
#include <bedrock/protocol/MotionPredictionHintsPacket.hpp>
#include <bedrock/protocol/AnimateEntityPacket.hpp>
#include <bedrock/protocol/CameraShakePacket.hpp>
#include <bedrock/protocol/PlayerFogPacket.hpp>
#include <bedrock/protocol/CorrectPlayerMovePredictionPacket.hpp>
#include <bedrock/protocol/ItemRegistryPacket.hpp>
#include <bedrock/protocol/ClientboundDebugRendererPacket.hpp>
#include <bedrock/protocol/SyncActorPropertyPacket.hpp>
#include <bedrock/protocol/AddVolumeEntityPacket.hpp>
#include <bedrock/protocol/RemoveVolumeEntityPacket.hpp>
#include <bedrock/protocol/SimulationTypePacket.hpp>
#include <bedrock/protocol/NpcDialoguePacket.hpp>
#include <bedrock/protocol/EduUriResourcePacket.hpp>
#include <bedrock/protocol/CreatePhotoPacket.hpp>
#include <bedrock/protocol/UpdateSubChunkBlocksPacket.hpp>
#include <bedrock/protocol/SubChunkPacket.hpp>
#include <bedrock/protocol/SubChunkRequestPacket.hpp>
#include <bedrock/protocol/PlayerStartItemCooldownPacket.hpp>
#include <bedrock/protocol/ScriptMessagePacket.hpp>
#include <bedrock/protocol/CodeBuilderSourcePacket.hpp>
#include <bedrock/protocol/TickingAreasLoadStatusPacket.hpp>
#include <bedrock/protocol/DimensionDataPacket.hpp>
#include <bedrock/protocol/AgentActionEventPacket.hpp>
#include <bedrock/protocol/ChangeMobPropertyPacket.hpp>
#include <bedrock/protocol/LessonProgressPacket.hpp>
#include <bedrock/protocol/RequestAbilityPacket.hpp>
#include <bedrock/protocol/RequestPermissionsPacket.hpp>
#include <bedrock/protocol/ToastRequestPacket.hpp>
#include <bedrock/protocol/UpdateAbilitiesPacket.hpp>
#include <bedrock/protocol/UpdateAdventureSettingsPacket.hpp>
#include <bedrock/protocol/DeathInfoPacket.hpp>
#include <bedrock/protocol/EditorNetworkPacket.hpp>
#include <bedrock/protocol/FeatureRegistryPacket.hpp>
#include <bedrock/protocol/ServerStatsPacket.hpp>
#include <bedrock/protocol/RequestNetworkSettingsPacket.hpp>
#include <bedrock/protocol/GameTestRequestPacket.hpp>
#include <bedrock/protocol/GameTestResultsPacket.hpp>
#include <bedrock/protocol/UpdateClientInputLocksPacket.hpp>
#include <bedrock/protocol/CameraPresetsPacket.hpp>
#include <bedrock/protocol/UnlockedRecipesPacket.hpp>
#include <bedrock/protocol/CameraInstructionPacket.hpp>
#include <bedrock/protocol/TrimDataPacket.hpp>
#include <bedrock/protocol/OpenSignPacket.hpp>
#include <bedrock/protocol/AgentAnimationPacket.hpp>
#include <bedrock/protocol/RefreshEntitlementsPacket.hpp>
#include <bedrock/protocol/PlayerToggleCrafterSlotRequestPacket.hpp>
#include <bedrock/protocol/SetPlayerInventoryOptionsPacket.hpp>
#include <bedrock/protocol/SetHudPacket.hpp>
#include <bedrock/protocol/AwardAchievementPacket.hpp>
#include <bedrock/protocol/ClientboundCloseFormPacket.hpp>
#include <bedrock/protocol/ServerboundLoadingScreenPacket.hpp>
#include <bedrock/protocol/JigsawStructureDataPacket.hpp>
#include <bedrock/protocol/CurrentStructureFeaturePacket.hpp>
#include <bedrock/protocol/ServerboundDiagnosticsPacket.hpp>
#include <bedrock/protocol/CameraAimAssistPacket.hpp>
#include <bedrock/protocol/ContainerRegistryCleanupPacket.hpp>
#include <bedrock/protocol/MovementEffectPacket.hpp>
#include <bedrock/protocol/CameraAimAssistPresetsPacket.hpp>
#include <bedrock/protocol/ClientCameraAimAssistPacket.hpp>
#include <bedrock/protocol/ClientMovementPredictionSyncPacket.hpp>
#include <bedrock/protocol/UpdateClientOptionsPacket.hpp>
#include <bedrock/protocol/PlayerVideoCapturePacket.hpp>
#include <bedrock/protocol/PlayerUpdateEntityOverridesPacket.hpp>
#include <bedrock/protocol/PlayerLocationPacket.hpp>
#include <bedrock/protocol/ClientboundControlSchemeSetPacket.hpp>
#include <bedrock/protocol/DebugDrawerPacket.hpp>
#include <bedrock/protocol/ServerboundPackSettingChangePacket.hpp>
#include <bedrock/protocol/ClientboundDataStorePacket.hpp>
#include <bedrock/protocol/GraphicsOverrideParameterPacket.hpp>
#include <bedrock/protocol/ServerboundDataStorePacket.hpp>
#include <bedrock/protocol/ClientboundDataDrivenUIShowScreenPacket.hpp>
#include <bedrock/protocol/ClientboundDataDrivenUICloseScreenPacket.hpp>
#include <bedrock/protocol/ClientboundDataDrivenUIReloadPacket.hpp>
#include <bedrock/protocol/ClientboundTextureShiftPacket.hpp>
#include <bedrock/protocol/VoxelShapesPacket.hpp>
#include <bedrock/protocol/CameraSplinePacket.hpp>
#include <bedrock/protocol/CameraAimAssistActorPriorityPacket.hpp>
#include <bedrock/protocol/ResourcePacksReadyForValidationPacket.hpp>
#include <bedrock/protocol/LocatorBarPacket.hpp>
#include <bedrock/protocol/PartyChangedPacket.hpp>
#include <bedrock/protocol/ServerboundDataDrivenScreenClosedPacket.hpp>
#include <bedrock/protocol/SyncWorldClocksPacket.hpp>
#include <bedrock/protocol/ClientboundAttributeLayerSyncPacket.hpp>
namespace bedrock::protocol {


void PacketPool::setup() noexcept {
    std::call_once(s_setupFlag, [](){
        {
            std::unique_lock lock(s_mutex);
            s_byId.reserve(ExpectedPacketCount);
            s_byType.reserve(ExpectedPacketCount);
        }
        registerPacket<LoginPacket>();
        registerPacket<PlayStatusPacket>();
        registerPacket<ServerToClientHandshakePacket>();
        registerPacket<ClientToServerHandshakePacket>();
        registerPacket<DisconnectPacket>();
        registerPacket<ResourcePacksInfoPacket>();
        registerPacket<ResourcePackStackPacket>();
        registerPacket<ResourcePackClientResponsePacket>();
        registerPacket<TextPacket>();
        registerPacket<SetTimePacket>();
        registerPacket<StartGamePacket>();
        registerPacket<AddPlayerPacket>();
        registerPacket<AddActorPacket>();
        registerPacket<RemoveActorPacket>();
        registerPacket<AddItemActorPacket>();
        registerPacket<ServerPlayerPostMovePositionPacket>();
        registerPacket<TakeItemActorPacket>();
        registerPacket<MoveActorAbsolutePacket>();
        registerPacket<MovePlayerPacket>();
        registerPacket<UpdateBlockPacket>();
        registerPacket<AddPaintingPacket>();
        registerPacket<LevelEventPacket>();
        registerPacket<BlockEventPacket>();
        registerPacket<ActorEventPacket>();
        registerPacket<MobEffectPacket>();
        registerPacket<UpdateAttributesPacket>();
        registerPacket<InventoryTransactionPacket>();
        registerPacket<MobEquipmentPacket>();
        registerPacket<MobArmorEquipmentPacket>();
        registerPacket<InteractPacket>();
        registerPacket<BlockPickRequestPacket>();
        registerPacket<ActorPickRequestPacket>();
        registerPacket<PlayerActionPacket>();
        registerPacket<HurtArmorPacket>();
        registerPacket<SetActorDataPacket>();
        registerPacket<SetActorMotionPacket>();
        registerPacket<SetActorLinkPacket>();
        registerPacket<SetHealthPacket>();
        registerPacket<SetSpawnPositionPacket>();
        registerPacket<AnimatePacket>();
        registerPacket<RespawnPacket>();
        registerPacket<ContainerOpenPacket>();
        registerPacket<ContainerClosePacket>();
        registerPacket<PlayerHotbarPacket>();
        registerPacket<InventoryContentPacket>();
        registerPacket<InventorySlotPacket>();
        registerPacket<ContainerSetDataPacket>();
        registerPacket<CraftingDataPacket>();
        registerPacket<GuiDataPickItemPacket>();
        registerPacket<BlockActorDataPacket>();
        registerPacket<LevelChunkPacket>();
        registerPacket<SetCommandsEnabledPacket>();
        registerPacket<SetDifficultyPacket>();
        registerPacket<ChangeDimensionPacket>();
        registerPacket<SetPlayerGameTypePacket>();
        registerPacket<PlayerListPacket>();
        registerPacket<SimpleEventPacket>();
        registerPacket<LegacyTelemetryEventPacket>();
        registerPacket<SpawnExperienceOrbPacket>();
        registerPacket<ClientboundMapItemDataPacket>();
        registerPacket<MapInfoRequestPacket>();
        registerPacket<RequestChunkRadiusPacket>();
        registerPacket<ChunkRadiusUpdatedPacket>();
        registerPacket<GameRulesChangedPacket>();
        registerPacket<CameraPacket>();
        registerPacket<BossEventPacket>();
        registerPacket<ShowCreditsPacket>();
        registerPacket<AvailableCommandsPacket>();
        registerPacket<CommandRequestPacket>();
        registerPacket<CommandBlockUpdatePacket>();
        registerPacket<CommandOutputPacket>();
        registerPacket<UpdateTradePacket>();
        registerPacket<UpdateEquipPacket>();
        registerPacket<ResourcePackDataInfoPacket>();
        registerPacket<ResourcePackChunkDataPacket>();
        registerPacket<ResourcePackChunkRequestPacket>();
        registerPacket<TransferPacket>();
        registerPacket<PlaySoundPacket>();
        registerPacket<StopSoundPacket>();
        registerPacket<SetTitlePacket>();
        registerPacket<AddBehaviorTreePacket>();
        registerPacket<StructureBlockUpdatePacket>();
        registerPacket<ShowStoreOfferPacket>();
        registerPacket<PurchaseReceiptPacket>();
        registerPacket<PlayerSkinPacket>();
        registerPacket<SubClientLoginPacket>();
        registerPacket<AutomationClientConnectPacket>();
        registerPacket<SetLastHurtByPacket>();
        registerPacket<BookEditPacket>();
        registerPacket<NpcRequestPacket>();
        registerPacket<PhotoTransferPacket>();
        registerPacket<ModalFormRequestPacket>();
        registerPacket<ModalFormResponsePacket>();
        registerPacket<ServerSettingsRequestPacket>();
        registerPacket<ServerSettingsResponsePacket>();
        registerPacket<ShowProfilePacket>();
        registerPacket<SetDefaultGameTypePacket>();
        registerPacket<RemoveObjectivePacket>();
        registerPacket<SetDisplayObjectivePacket>();
        registerPacket<SetScorePacket>();
        registerPacket<LabTablePacket>();
        registerPacket<UpdateBlockSyncedPacket>();
        registerPacket<MoveActorDeltaPacket>();
        registerPacket<SetScoreboardIdentityPacket>();
        registerPacket<SetLocalPlayerAsInitializedPacket>();
        registerPacket<UpdateSoftEnumPacket>();
        registerPacket<NetworkStackLatencyPacket>();
        registerPacket<SpawnParticleEffectPacket>();
        registerPacket<AvailableActorIdentifiersPacket>();
        registerPacket<NetworkChunkPublisherUpdatePacket>();
        registerPacket<BiomeDefinitionListPacket>();
        registerPacket<LevelSoundEventPacket>();
        registerPacket<LevelEventGenericPacket>();
        registerPacket<LecternUpdatePacket>();
        registerPacket<ClientCacheStatusPacket>();
        registerPacket<OnScreenTextureAnimationPacket>();
        registerPacket<MapCreateLockedCopyPacket>();
        registerPacket<StructureTemplateDataRequestPacket>();
        registerPacket<StructureTemplateDataResponsePacket>();
        registerPacket<ClientCacheBlobStatusPacket>();
        registerPacket<ClientCacheMissResponsePacket>();
        registerPacket<EducationSettingsPacket>();
        registerPacket<EmotePacket>();
        registerPacket<MultiplayerSettingsPacket>();
        registerPacket<SettingsCommandPacket>();
        registerPacket<AnvilDamagePacket>();
        registerPacket<CompletedUsingItemPacket>();
        registerPacket<NetworkSettingsPacket>();
        registerPacket<PlayerAuthInputPacket>();
        registerPacket<CreativeContentPacket>();
        registerPacket<PlayerEnchantOptionsPacket>();
        registerPacket<ItemStackRequestPacket>();
        registerPacket<ItemStackResponsePacket>();
        registerPacket<PlayerArmorDamagePacket>();
        registerPacket<CodeBuilderPacket>();
        registerPacket<UpdatePlayerGameTypePacket>();
        registerPacket<EmoteListPacket>();
        registerPacket<PositionTrackingDBServerBroadcastPacket>();
        registerPacket<PositionTrackingDBClientRequestPacket>();
        registerPacket<DebugInfoPacket>();
        registerPacket<PacketViolationWarningPacket>();
        registerPacket<MotionPredictionHintsPacket>();
        registerPacket<AnimateEntityPacket>();
        registerPacket<CameraShakePacket>();
        registerPacket<PlayerFogPacket>();
        registerPacket<CorrectPlayerMovePredictionPacket>();
        registerPacket<ItemRegistryPacket>();
        registerPacket<ClientboundDebugRendererPacket>();
        registerPacket<SyncActorPropertyPacket>();
        registerPacket<AddVolumeEntityPacket>();
        registerPacket<RemoveVolumeEntityPacket>();
        registerPacket<SimulationTypePacket>();
        registerPacket<NpcDialoguePacket>();
        registerPacket<EduUriResourcePacket>();
        registerPacket<CreatePhotoPacket>();
        registerPacket<UpdateSubChunkBlocksPacket>();
        registerPacket<SubChunkPacket>();
        registerPacket<SubChunkRequestPacket>();
        registerPacket<PlayerStartItemCooldownPacket>();
        registerPacket<ScriptMessagePacket>();
        registerPacket<CodeBuilderSourcePacket>();
        registerPacket<TickingAreasLoadStatusPacket>();
        registerPacket<DimensionDataPacket>();
        registerPacket<AgentActionEventPacket>();
        registerPacket<ChangeMobPropertyPacket>();
        registerPacket<LessonProgressPacket>();
        registerPacket<RequestAbilityPacket>();
        registerPacket<RequestPermissionsPacket>();
        registerPacket<ToastRequestPacket>();
        registerPacket<UpdateAbilitiesPacket>();
        registerPacket<UpdateAdventureSettingsPacket>();
        registerPacket<DeathInfoPacket>();
        registerPacket<EditorNetworkPacket>();
        registerPacket<FeatureRegistryPacket>();
        registerPacket<ServerStatsPacket>();
        registerPacket<RequestNetworkSettingsPacket>();
        registerPacket<GameTestRequestPacket>();
        registerPacket<GameTestResultsPacket>();
        registerPacket<UpdateClientInputLocksPacket>();
        registerPacket<CameraPresetsPacket>();
        registerPacket<UnlockedRecipesPacket>();
        registerPacket<CameraInstructionPacket>();
        registerPacket<TrimDataPacket>();
        registerPacket<OpenSignPacket>();
        registerPacket<AgentAnimationPacket>();
        registerPacket<RefreshEntitlementsPacket>();
        registerPacket<PlayerToggleCrafterSlotRequestPacket>();
        registerPacket<SetPlayerInventoryOptionsPacket>();
        registerPacket<SetHudPacket>();
        registerPacket<AwardAchievementPacket>();
        registerPacket<ClientboundCloseFormPacket>();
        registerPacket<ServerboundLoadingScreenPacket>();
        registerPacket<JigsawStructureDataPacket>();
        registerPacket<CurrentStructureFeaturePacket>();
        registerPacket<ServerboundDiagnosticsPacket>();
        registerPacket<CameraAimAssistPacket>();
        registerPacket<ContainerRegistryCleanupPacket>();
        registerPacket<MovementEffectPacket>();
        registerPacket<CameraAimAssistPresetsPacket>();
        registerPacket<ClientCameraAimAssistPacket>();
        registerPacket<ClientMovementPredictionSyncPacket>();
        registerPacket<UpdateClientOptionsPacket>();
        registerPacket<PlayerVideoCapturePacket>();
        registerPacket<PlayerUpdateEntityOverridesPacket>();
        registerPacket<PlayerLocationPacket>();
        registerPacket<ClientboundControlSchemeSetPacket>();
        registerPacket<DebugDrawerPacket>();
        registerPacket<ServerboundPackSettingChangePacket>();
        registerPacket<ClientboundDataStorePacket>();
        registerPacket<GraphicsOverrideParameterPacket>();
        registerPacket<ServerboundDataStorePacket>();
        registerPacket<ClientboundDataDrivenUIShowScreenPacket>();
        registerPacket<ClientboundDataDrivenUICloseScreenPacket>();
        registerPacket<ClientboundDataDrivenUIReloadPacket>();
        registerPacket<ClientboundTextureShiftPacket>();
        registerPacket<VoxelShapesPacket>();
        registerPacket<CameraSplinePacket>();
        registerPacket<CameraAimAssistActorPriorityPacket>();
        registerPacket<ResourcePacksReadyForValidationPacket>();
        registerPacket<LocatorBarPacket>();
        registerPacket<PartyChangedPacket>();
        registerPacket<ServerboundDataDrivenScreenClosedPacket>();
        registerPacket<SyncWorldClocksPacket>();
        registerPacket<ClientboundAttributeLayerSyncPacket>();
    });
}

template<typename P>
requires std::derived_from<P, Packet>
void PacketPool::registerPacket() noexcept {
    const entt::id_type tid = entt::type_hash<P>::value();

    std::unique_lock lock(s_mutex);

    if (s_byType.find(tid) != s_byType.end()) return;

    s_byType.emplace(tid, &PacketPool::ctor<P>);
    s_byId.emplace(P::ID, &PacketPool::ctor<P>);
}

} // namespace bedrock::protocol