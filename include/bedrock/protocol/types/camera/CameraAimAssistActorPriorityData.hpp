#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class CameraAimAssistActorPriorityData {
public:
    int32_t presetIndex;
    int32_t categoryIndex;
    int32_t actorIndex;
    int32_t priority;

    CameraAimAssistActorPriorityData() = default;
    explicit CameraAimAssistActorPriorityData(
        const int32_t& presetIndexValue,
        const int32_t& categoryIndexValue,
        const int32_t& actorIndexValue,
        const int32_t& priorityValue
    ) :
        presetIndex(presetIndexValue),
        categoryIndex(categoryIndexValue),
        actorIndex(actorIndexValue),
        priority(priorityValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        presetIndex = stream.readInt().value_or(0);
        categoryIndex = stream.readInt().value_or(0);
        actorIndex = stream.readInt().value_or(0);
        priority = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeInt(presetIndex);
		stream.writeInt(categoryIndex);
		stream.writeInt(actorIndex);
		stream.writeInt(priority);
    }
};
