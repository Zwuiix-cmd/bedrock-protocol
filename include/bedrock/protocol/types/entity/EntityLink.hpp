#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>

namespace bedrock::protocol {


class EntityLink {
public:
    enum class Type : uint8_t {
        Remove = 0,
        Rider = 1,
        Passenger = 2,
    };

    int64_t fromActorUniqueId;
    int64_t toActorUniqueId;
    Type type;
    bool immediate;
    bool causedByRider;
    float vehicleAngularVelocity;

    EntityLink() = default;
    explicit EntityLink(
        const int64_t& fromActorUniqueIdValue,
        const int64_t& toActorUniqueIdValue,
        const Type& typeValue,
        const bool& immediateValue,
        const bool& causedByRiderValue,
        const float& vehicleAngularVelocityValue
    ) :
        fromActorUniqueId(fromActorUniqueIdValue),
        toActorUniqueId(toActorUniqueIdValue),
        type(typeValue),
        immediate(immediateValue),
        causedByRider(causedByRiderValue),
        vehicleAngularVelocity(vehicleAngularVelocityValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        fromActorUniqueId = stream.readActorUniqueID().value_or(0);
        toActorUniqueId = stream.readActorUniqueID().value_or(0);
        type = magic_enum::enum_cast<Type>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(Type::Remove);
        immediate = stream.readBool().value_or(false);
        causedByRider = stream.readBool().value_or(false);
        vehicleAngularVelocity = stream.readFloat().value_or(0.0f);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeActorUniqueID(fromActorUniqueId);
        stream.writeActorUniqueID(toActorUniqueId);
        stream.writeUnsignedByte(magic_enum::enum_integer(type));
        stream.writeBool(immediate);
        stream.writeBool(causedByRider);
        stream.writeFloat(vehicleAngularVelocity);
    }
};

} // namespace bedrock::protocol
