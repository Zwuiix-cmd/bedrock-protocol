#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

namespace bedrock::protocol {


class StructureSettings {
public:
    std::string paletteName;
    bool ignoreEntities;
    bool ignoreBlocks;
    bool allowNonTickingChunks;
    glm::ivec3 dimensions;
    glm::ivec3 offset;
    int64_t lastTouchedByPlayerID;
    uint8_t rotation;
    uint8_t mirror;
    uint8_t animationMode;
    float animationSeconds;
    float integrityValue;
    uint32_t integritySeed;
    glm::vec3 pivot;

    StructureSettings() = default;

    virtual void read(ReadOnlyBinaryStream& stream) {
        paletteName = stream.readString().value_or("");
        ignoreEntities = stream.readBool().value_or(false);
        ignoreBlocks = stream.readBool().value_or(false);
        allowNonTickingChunks = stream.readBool().value_or(false);
        dimensions = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        offset = stream.readSignedBlockPos().value_or(glm::ivec3(0));
        lastTouchedByPlayerID = stream.readActorUniqueID().value_or(0);
        rotation = stream.readUnsignedByte().value_or(0);
        mirror = stream.readUnsignedByte().value_or(0);
        animationMode = stream.readUnsignedByte().value_or(0);
        animationSeconds = stream.readFloat().value_or(0.0f);
        integrityValue = stream.readFloat().value_or(0.0f);
        integritySeed = stream.readUnsignedInt().value_or(0);
        pivot = stream.readVec3().value_or(glm::vec3(0.0f));
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeString(paletteName);
        stream.writeBool(ignoreEntities);
        stream.writeBool(ignoreBlocks);
        stream.writeBool(allowNonTickingChunks);
        stream.writeSignedBlockPos(dimensions);
        stream.writeSignedBlockPos(offset);
        stream.writeActorUniqueID(lastTouchedByPlayerID);
        stream.writeUnsignedByte(rotation);
        stream.writeUnsignedByte(mirror);
        stream.writeUnsignedByte(animationMode);
        stream.writeFloat(animationSeconds);
        stream.writeFloat(integrityValue);
        stream.writeUnsignedInt(integritySeed);
        stream.writeVec3(pivot);
    }
};

} // namespace bedrock::protocol
