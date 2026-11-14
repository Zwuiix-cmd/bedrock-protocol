#pragma once

#include <bedrock/protocol/types/TypesGeneratedIncludes0.hpp>
#include <bedrock/protocol/types/TypesGeneratedIncludes1.hpp>

namespace bedrock::protocol {


inline std::shared_ptr<AttributeEnvironmentPayload> AttributeEnvironmentPayload::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 0: {
            auto value = std::make_shared<AttributeEnvironmentConstant>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<AttributeEnvironmentNoiseTransition>();
            value->read(stream);
            return value;
        }
        case 1: {
            auto value = std::make_shared<AttributeEnvironmentTransition>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<AttributeLayerSettingsWeight> AttributeLayerSettingsWeight::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 0: {
            auto value = std::make_shared<AttributeLayerSettingsWeightFloat>();
            value->read(stream);
            return value;
        }
        case 1: {
            auto value = std::make_shared<AttributeLayerSettingsWeightString>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<AttributeLayerSyncPayload> AttributeLayerSyncPayload::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<AttributeUpdateLayerSettings>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<AttributeUpdateLayers>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<AttributesRemoveEnvironment>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<AttributesUpdateEnvironment>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<AttributeValue> AttributeValue::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 0: {
            auto value = std::make_shared<AttributeValueBool>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<AttributeValueColor>();
            value->read(stream);
            return value;
        }
        case 1: {
            auto value = std::make_shared<AttributeValueFloat>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<AttributeValueColorValue> AttributeValueColorValue::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<AttributeValueColorArray>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<AttributeValueColorString>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<DataStoreUpdateValue> DataStoreUpdateValue::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<BoolDataStoreUpdateValue>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<DoubleDataStoreUpdateValue>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<StringDataStoreUpdateValue>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<DynamicValue> DynamicValue::create(ReadOnlyBinaryStream& stream) {
    return create(stream, stream.readUnsignedInt().value_or(0));
}

inline std::shared_ptr<DynamicValue> DynamicValue::create(ReadOnlyBinaryStream& stream, uint32_t type) {
    switch (type) {
        case 1: {
            auto value = std::make_shared<DynamicValueBool>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<DynamicValueDouble>();
            value->read(stream);
            return value;
        }
        case 5: {
            auto value = std::make_shared<DynamicValueList>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<DynamicValueLong>();
            value->read(stream);
            return value;
        }
        case 6: {
            auto value = std::make_shared<DynamicValueMap>();
            value->read(stream);
            return value;
        }
        case 4: {
            auto value = std::make_shared<DynamicValueString>();
            value->read(stream);
            return value;
        }
        case 0: return nullptr;
        default: stream.markOverflowed(); return nullptr;
    }
}

inline std::shared_ptr<GameRule> GameRule::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<BoolGameRule>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<FloatGameRule>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<IntGameRule>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<NullGameRule>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<GameRule> GameRule::create(ReadOnlyBinaryStream& stream, const bool& isPlayerModifiable, const bool& isStartGame) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: return BoolGameRule::create(stream, isPlayerModifiable);
        case 3: return FloatGameRule::create(stream, isPlayerModifiable);
        case 2: return IntGameRule::create(stream, isPlayerModifiable);
        case 0: return NullGameRule::create(stream, isPlayerModifiable);
        default: return nullptr;
    }
}

inline std::map<std::string, std::shared_ptr<GameRule>> GameRule::readRules(ReadOnlyBinaryStream& stream, const bool& isStartGame) {
    std::map<std::string, std::shared_ptr<GameRule>> rules;
    const auto count = stream.readUnsignedVarInt().value_or(0);
    const bool countAvailable = stream.canReadCollection(count);
    for (uint32_t index = 0; index < count && countAvailable && !stream.hasOverflowed(); ++index) {
        auto name = stream.readString().value_or("");
        const auto isPlayerModifiable = stream.readBool().value_or(false);
        rules.emplace(std::move(name), GameRule::create(stream, isPlayerModifiable, isStartGame));
    }
    return rules;
}

inline void GameRule::writeRules(BinaryStream& stream, const std::map<std::string, std::shared_ptr<GameRule>>& rules, const bool& isStartGame) {
    stream.writeUnsignedVarInt(static_cast<uint32_t>(rules.size()));
    for (const auto& [name, rule] : rules) {
        stream.writeString(name);
        if (!rule) continue;
        stream.writeBool(rule->isPlayerModifiable);
        stream.writeUnsignedVarInt(static_cast<uint32_t>(rule->getTypeId()));
        rule->write(stream);
    }
}

inline std::shared_ptr<ItemStackRequestAction> ItemStackRequestAction::create(ReadOnlyBinaryStream& stream) {
    return create(stream, stream.readUnsignedVarInt().value_or(0));
}

inline std::shared_ptr<ItemStackRequestAction> ItemStackRequestAction::create(ReadOnlyBinaryStream& stream, uint32_t type) {
    switch (type) {
        case 8: {
            auto value = std::make_shared<BeaconPaymentStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 11: {
            auto value = std::make_shared<CraftRecipeAutoStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 13: {
            auto value = std::make_shared<CraftRecipeOptionalStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 10: {
            auto value = std::make_shared<CraftRecipeStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 16: {
            auto value = std::make_shared<CraftReservedStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 5: {
            auto value = std::make_shared<CraftingConsumeInputStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 6: {
            auto value = std::make_shared<CraftingCreateSpecificResultStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 12: {
            auto value = std::make_shared<CreativeCreateStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 17: {
            auto value = std::make_shared<DeprecatedCraftingNonImplementedStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 18: {
            auto value = std::make_shared<DeprecatedCraftingResultsStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 4: {
            auto value = std::make_shared<DestroyStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<DropStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 14: {
            auto value = std::make_shared<GrindstoneStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 7: {
            auto value = std::make_shared<LabTableCombineStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 15: {
            auto value = std::make_shared<LoomStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 9: {
            auto value = std::make_shared<MineBlockStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 1: {
            auto value = std::make_shared<PlaceStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<SwapStackRequestAction>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<TakeStackRequestAction>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<PackSetting> PackSetting::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 3: {
            auto value = std::make_shared<StringArrayPackSetting>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<StringPackSetting>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<PrimitiveShapePayload> PrimitiveShapePayload::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<PrimitiveShapeArrowPayload>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<PrimitiveShapeBoxPayload>();
            value->read(stream);
            return value;
        }
        case 5: {
            auto value = std::make_shared<PrimitiveShapeCircleOrSpherePayload>();
            value->read(stream);
            return value;
        }
        case 9: {
            auto value = std::make_shared<PrimitiveShapeConePayload>();
            value->read(stream);
            return value;
        }
        case 6: {
            auto value = std::make_shared<PrimitiveShapeCylinderPayload>();
            value->read(stream);
            return value;
        }
        case 8: {
            auto value = std::make_shared<PrimitiveShapeEllipsoidPayload>();
            value->read(stream);
            return value;
        }
        case 4: {
            auto value = std::make_shared<PrimitiveShapeLinePayload>();
            value->read(stream);
            return value;
        }
        case 7: {
            auto value = std::make_shared<PrimitiveShapePyramidPayload>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<PrimitiveShapeTextPayload>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<SyncWorldClocksPayload> SyncWorldClocksPayload::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 2: {
            auto value = std::make_shared<SyncWorldClocksAddTimeMarker>();
            value->read(stream);
            return value;
        }
        case 1: {
            auto value = std::make_shared<SyncWorldClocksInitializeRegistry>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<SyncWorldClocksRemoveTimeMarker>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<SyncWorldClocksSyncState>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<TransactionData> TransactionData::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<MismatchTransactionData>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<NormalTransactionData>();
            value->read(stream);
            return value;
        }
        case 4: {
            auto value = std::make_shared<ReleaseItemTransactionData>();
            value->read(stream);
            return value;
        }
        case 3: {
            auto value = std::make_shared<UseItemOnEntityTransactionData>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<UseItemTransactionData>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

inline std::shared_ptr<DataStoreOperation> DataStoreOperation::create(ReadOnlyBinaryStream& stream) {
    switch (stream.readUnsignedVarInt().value_or(0)) {
        case 1: {
            auto value = std::make_shared<DataStoreChange>();
            value->read(stream);
            return value;
        }
        case 2: {
            auto value = std::make_shared<DataStoreRemoval>();
            value->read(stream);
            return value;
        }
        case 0: {
            auto value = std::make_shared<DataStoreUpdate>();
            value->read(stream);
            return value;
        }
        default:
            return nullptr;
    }
}

} // namespace bedrock::protocol
