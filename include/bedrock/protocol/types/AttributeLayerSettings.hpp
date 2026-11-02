#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/AttributeLayerSettingsWeight.hpp>
#include <bedrock/protocol/types/AttributeLayerSettingsWeightFloat.hpp>
#include <bedrock/protocol/types/AttributeLayerSettingsWeightString.hpp>

class AttributeLayerSettings {
public:
    int32_t priority;
    std::shared_ptr<AttributeLayerSettingsWeight> weight;
    bool enabled;
    bool transitionsPaused;

    AttributeLayerSettings() = default;
    explicit AttributeLayerSettings(
        const int32_t& priorityValue,
        const std::shared_ptr<AttributeLayerSettingsWeight>& weightValue,
        const bool& enabledValue,
        const bool& transitionsPausedValue
    ) :
        priority(priorityValue),
        weight(weightValue),
        enabled(enabledValue),
        transitionsPaused(transitionsPausedValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        priority = stream.readInt().value_or(0);
        weight = AttributeLayerSettingsWeight::create(stream);
        enabled = stream.readBool().value_or(false);
        transitionsPaused = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeInt(priority);
		stream.writeUnsignedVarInt(weight->getTypeId());
		weight->write(stream);
		stream.writeBool(enabled);
		stream.writeBool(transitionsPaused);
    }
};
