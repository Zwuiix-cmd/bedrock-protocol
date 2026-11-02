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
#include <bedrock/protocol/types/LocatorBarWaypoint.hpp>
#include <bedrock/protocol/types/LocatorBarWaypoint.hpp>
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
        action = stream.readUnsignedByte().value_or(0);
        group = stream.readUUID().value_or("");
        waypoint = ([&]() {
    auto value = std::make_shared<LocatorBarWaypoint>();
    value->read(stream);
    return value;
}());
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUUID(group);
		waypoint->write(stream);
		stream.writeUnsignedByte(action);
    }
};
