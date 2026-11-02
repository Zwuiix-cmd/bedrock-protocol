#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>


class WorldPosition {
public:
    glm::vec3 position;
    int32_t dimension;

    WorldPosition() = default;
    explicit WorldPosition(const glm::vec3& positionValue, const int32_t& dimensionValue) : position(positionValue), dimension(dimensionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        position = stream.readVec3().value_or(glm::vec3(0.0f));
        dimension = stream.readVarInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeVec3(position);
		stream.writeVarInt(dimension);
    }
};
