#pragma once

#include <cstdint>
#include <magic_enum/magic_enum.hpp>

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>

enum class AbilitiesLayerId : uint16_t {
    Cache = 0,
    Base = 1,
    Spectator = 2,
    Commands = 3,
    Editor = 4,
    LoadingScreen = 5,
};

class AbilitiesLayer {
public:
    AbilitiesLayerId layerId = AbilitiesLayerId::Base;
    uint32_t setAbilities = 0;
    uint32_t setAbilityValues = 0;
    float flySpeed = 0.0f;
    float verticalFlySpeed = 0.0f;
    float walkSpeed = 0.0f;

    void read(ReadOnlyBinaryStream& stream) {
        layerId = static_cast<AbilitiesLayerId>(stream.readUnsignedShort().value_or(0));
        setAbilities = stream.readUnsignedInt().value_or(0);
        setAbilityValues = stream.readUnsignedInt().value_or(0);
        flySpeed = stream.readFloat().value_or(0.0f);
        verticalFlySpeed = stream.readFloat().value_or(0.0f);
        walkSpeed = stream.readFloat().value_or(0.0f);
    }

    void write(BinaryStream& stream) const {
        stream.writeUnsignedShort(magic_enum::enum_integer(layerId));
        stream.writeUnsignedInt(setAbilities);
        stream.writeUnsignedInt(setAbilityValues);
        stream.writeFloat(flySpeed);
        stream.writeFloat(verticalFlySpeed);
        stream.writeFloat(walkSpeed);
    }
};