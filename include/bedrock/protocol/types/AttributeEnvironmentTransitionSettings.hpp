#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/camera/CameraSetInstructionEaseType.hpp>
namespace bedrock::protocol {


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
        easeType = magic_enum::enum_cast<CameraSetInstructionEaseType>(
            stream.readVarInt().value_or(0)
        ).value_or(CameraSetInstructionEaseType::Linear);
        clockName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedVarInt(totalTransitionTicks);
		stream.writeUnsignedVarInt(currentTransitionTicks);
		stream.writeVarInt(magic_enum::enum_integer(easeType));
		stream.writeString(clockName);
    }
};

} // namespace bedrock::protocol
