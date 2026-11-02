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

class GatheringJoinInfo {
public:
    std::string experienceId;
    std::string experienceName;
    std::optional<std::string> experienceWorldId;
    std::optional<std::string> experienceWorldName;
    std::string creatorId;
    std::optional<std::string> targetId;
    std::optional<std::string> scenarioId;
    std::optional<std::string> serverId;

    GatheringJoinInfo() = default;
    explicit GatheringJoinInfo(
        const std::string& experienceIdValue,
        const std::string& experienceNameValue,
        const std::optional<std::string>& experienceWorldIdValue,
        const std::optional<std::string>& experienceWorldNameValue,
        const std::string& creatorIdValue,
        const std::optional<std::string>& targetIdValue,
        const std::optional<std::string>& scenarioIdValue,
        const std::optional<std::string>& serverIdValue
    ) :
        experienceId(experienceIdValue),
        experienceName(experienceNameValue),
        experienceWorldId(experienceWorldIdValue),
        experienceWorldName(experienceWorldNameValue),
        creatorId(creatorIdValue),
        targetId(targetIdValue),
        scenarioId(scenarioIdValue),
        serverId(serverIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        experienceName = stream.readString().value_or("");
        creatorId = stream.readString().value_or("");
        experienceId = stream.readUUID().value_or("");
        experienceWorldId = stream.readOptional([&]() {
			return stream.readUUID().value_or("");
		});
        experienceWorldName = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        targetId = stream.readOptional([&]() {
			return stream.readUUID().value_or("");
		});
        scenarioId = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        serverId = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUUID(experienceId);
		stream.writeString(experienceName);
		stream.writeOptional(experienceWorldId, [&](const auto& value) {
					stream.writeUUID(value);
				});
		stream.writeOptional(experienceWorldName, [&](const auto& value) {
					stream.writeString(value);
				});
		stream.writeString(creatorId);
		stream.writeOptional(targetId, [&](const auto& value) {
					stream.writeUUID(value);
				});
		stream.writeOptional(scenarioId, [&](const auto& value) {
					stream.writeString(value);
				});
		stream.writeOptional(serverId, [&](const auto& value) {
					stream.writeString(value);
				});
    }
};
