#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

namespace bedrock::protocol {


class PlayerAuthInputVehicleInfo {
public:
    glm::vec2 vehicleRotation;
    int64_t predictedVehicleActorUniqueId;

    PlayerAuthInputVehicleInfo() = default;
    explicit PlayerAuthInputVehicleInfo(
        const glm::vec2& vehicleRotationValue,
        const int64_t& predictedVehicleActorUniqueIdValue
    ) :
        vehicleRotation(vehicleRotationValue),
        predictedVehicleActorUniqueId(predictedVehicleActorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        vehicleRotation = stream.readVec2().value_or(glm::vec2(0.0f));
        predictedVehicleActorUniqueId = stream.readActorUniqueID().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeVec2(vehicleRotation);
        stream.writeActorUniqueID(predictedVehicleActorUniqueId);
    }
};

} // namespace bedrock::protocol
