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
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>

class AttributeEnvironmentTransitionSettings {
public:
    uint32_t totalTransitionTicks;
    uint32_t currentTransitionTicks;
    CameraSetInstructionEaseType easeType;
    std::string clockName;

    AttributeEnvironmentTransitionSettings() = default;
    explicit AttributeEnvironmentTransitionSettings(
        const uint32_t& totalTransitionTicksValue,
        const uint32_t& currentTransitionTicksValue,
        const CameraSetInstructionEaseType& easeTypeValue,
        const std::string& clockNameValue
    ) :
        totalTransitionTicks(totalTransitionTicksValue),
        currentTransitionTicks(currentTransitionTicksValue),
        easeType(easeTypeValue),
        clockName(clockNameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        totalTransitionTicks = stream.readUnsignedVarInt().value_or(0);
        currentTransitionTicks = stream.readUnsignedVarInt().value_or(0);
        clockName = stream.readString().value_or("");
        easeType = magic_enum::enum_cast<CameraSetInstructionEaseType>(stream.readVarInt().value_or(0)).value_or(CameraSetInstructionEaseType::Linear);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarInt(totalTransitionTicks);
        stream.writeUnsignedVarInt(currentTransitionTicks);
        stream.writeString(clockName);
        stream.writeVarInt(magic_enum::enum_integer(easeType));
    }
};
