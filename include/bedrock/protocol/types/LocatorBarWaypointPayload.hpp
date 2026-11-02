#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>

#include <bedrock/protocol/types/LocatorBarWaypoint.hpp>

class LocatorBarWaypointPayload {
public:
    std::string group;
    std::shared_ptr<LocatorBarWaypoint> waypoint;
    uint8_t action;

    LocatorBarWaypointPayload() = default;
    explicit LocatorBarWaypointPayload(
        const std::string& groupValue,
        const std::shared_ptr<LocatorBarWaypoint>& waypointValue,
        const uint8_t& actionValue
    ) :
        group(groupValue),
        waypoint(waypointValue),
        action(actionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        group = stream.readUUID().value_or("");
        waypoint = std::make_shared<LocatorBarWaypoint>();
        waypoint->read(stream);
        action = stream.readUnsignedByte().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUUID(group);
		waypoint->write(stream);
		stream.writeUnsignedByte(action);
    }
};
