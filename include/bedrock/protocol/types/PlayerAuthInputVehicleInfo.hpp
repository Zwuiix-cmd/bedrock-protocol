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

class PlayerAuthInputVehicleInfo {
public:
    glm::vec2 vehicleRotation;
    int32_t predictedVehicleActorUniqueId;

    PlayerAuthInputVehicleInfo() = default;
    explicit PlayerAuthInputVehicleInfo(
        const glm::vec2& vehicleRotationValue,
        const int32_t& predictedVehicleActorUniqueIdValue
    ) :
        vehicleRotation(vehicleRotationValue),
        predictedVehicleActorUniqueId(predictedVehicleActorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        vehicleRotation = stream.readVec2().value_or(glm::vec2(0.0f));
        predictedVehicleActorUniqueId = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVec2(vehicleRotation);
        stream.writeInt(predictedVehicleActorUniqueId);
    }
};
