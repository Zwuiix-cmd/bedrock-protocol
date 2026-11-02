#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>

#include <bedrock/protocol/types/PlayerAuthInputVehicleInfo.hpp>
namespace bedrock::protocol {


class PlayerAuthInputVehicleInfo {
public:
    float vehicleRotationX;
    float vehicleRotationZ;
    int64_t predictedVehicleActorUniqueId;

    PlayerAuthInputVehicleInfo() = default;
    explicit PlayerAuthInputVehicleInfo(
        const float& vehicleRotationXValue,
        const float& vehicleRotationZValue,
        const int64_t& predictedVehicleActorUniqueIdValue
    ) :
        vehicleRotationX(vehicleRotationXValue),
        vehicleRotationZ(vehicleRotationZValue),
        predictedVehicleActorUniqueId(predictedVehicleActorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        vehicleRotationX = stream.readFloat().value_or(0.0f);
        vehicleRotationZ = stream.readFloat().value_or(0.0f);
        predictedVehicleActorUniqueId = stream.readActorUniqueID().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeFloat(vehicleRotationX);
		stream.writeFloat(vehicleRotationZ);
		stream.writeActorUniqueID(predictedVehicleActorUniqueId);
    }
};

} // namespace bedrock::protocol
