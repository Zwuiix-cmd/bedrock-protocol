#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {

class AttributeLayerSettings {
public:
    int32_t priority = 0;
    float floatWeight = 0.0f;
    bool enabled = false;
    bool transitionsPaused = false;

    AttributeLayerSettings() = default;
    explicit AttributeLayerSettings(
        int32_t priorityValue,
        float floatWeightValue,
        bool enabledValue,
        bool transitionsPausedValue
    ) :
        priority(priorityValue),
        floatWeight(floatWeightValue),
        enabled(enabledValue),
        transitionsPaused(transitionsPausedValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        priority = stream.readInt().value_or(0);
        floatWeight = stream.readFloat().value_or(0.0f);
        enabled = stream.readBool().value_or(false);
        transitionsPaused = stream.readBool().value_or(false);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(priority);
        stream.writeFloat(floatWeight);
        stream.writeBool(enabled);
        stream.writeBool(transitionsPaused);
    }
};

} // namespace bedrock::protocol
