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
#include <bedrock/protocol/types/NoiseAlignment.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/NoiseAlignment.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/NoiseAlignment.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>

class AttributeEnvironmentNoiseTransitionSettings {
public:
    uint32_t totalTransitionTicks;
    uint32_t currentTransitionTicks;
    CameraSetInstructionEaseType easeType;
    std::string clockName;
    uint32_t localTransitionTicks;
    std::string noiseName;
    std::shared_ptr<NoiseAlignment> noiseAlignment;

    AttributeEnvironmentNoiseTransitionSettings() = default;
    explicit AttributeEnvironmentNoiseTransitionSettings(
        const uint32_t& totalTransitionTicksValue,
        const uint32_t& currentTransitionTicksValue,
        const CameraSetInstructionEaseType& easeTypeValue,
        const std::string& clockNameValue,
        const uint32_t& localTransitionTicksValue,
        const std::string& noiseNameValue,
        const std::shared_ptr<NoiseAlignment>& noiseAlignmentValue
    ) :
        totalTransitionTicks(totalTransitionTicksValue),
        currentTransitionTicks(currentTransitionTicksValue),
        easeType(easeTypeValue),
        clockName(clockNameValue),
        localTransitionTicks(localTransitionTicksValue),
        noiseName(noiseNameValue),
        noiseAlignment(noiseAlignmentValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        totalTransitionTicks = stream.readUnsignedVarInt().value_or(0);
        currentTransitionTicks = stream.readUnsignedVarInt().value_or(0);
        clockName = stream.readString().value_or("");
        localTransitionTicks = stream.readUnsignedVarInt().value_or(0);
        noiseName = stream.readString().value_or("");
        easeType = static_cast<CameraSetInstructionEaseType>(stream.readVarInt().value_or(0));
        noiseAlignment = ([&]() {
    auto value = std::make_shared<NoiseAlignment>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(totalTransitionTicks);
        stream.writeUnsignedVarInt(currentTransitionTicks);
        stream.writeString(clockName);
        stream.writeUnsignedVarInt(localTransitionTicks);
        stream.writeString(noiseName);
        stream.writeVarInt(static_cast<int32_t>(easeType));
        if (noiseAlignment) noiseAlignment->write(stream);
    }
};
