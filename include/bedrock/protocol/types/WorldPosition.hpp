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
