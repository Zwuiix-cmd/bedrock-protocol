#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>


class ServerTelemetryData {
public:
    std::string serverId;
    std::string scenarioId;
    std::string worldId;
    std::string ownerId;

    ServerTelemetryData() = default;
    explicit ServerTelemetryData(
        const std::string& serverIdValue,
        const std::string& scenarioIdValue,
        const std::string& worldIdValue,
        const std::string& ownerIdValue
    ) :
        serverId(serverIdValue),
        scenarioId(scenarioIdValue),
        worldId(worldIdValue),
        ownerId(ownerIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        serverId = stream.readString().value_or("");
        scenarioId = stream.readString().value_or("");
        worldId = stream.readString().value_or("");
        ownerId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(serverId);
		stream.writeString(scenarioId);
		stream.writeString(worldId);
		stream.writeString(ownerId);
    }
};
