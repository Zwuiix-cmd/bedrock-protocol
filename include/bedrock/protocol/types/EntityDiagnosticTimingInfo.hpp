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

class EntityDiagnosticTimingInfo {
public:
    std::string displayName;
    std::string entity;
    uint64_t timeInNS;
    uint8_t percentOfTotal;
    std::optional<glm::vec3> position;
    std::optional<std::string> dimension;

    EntityDiagnosticTimingInfo() = default;
    explicit EntityDiagnosticTimingInfo(
        const std::string& displayNameValue,
        const std::string& entityValue,
        const uint64_t& timeInNSValue,
        const uint8_t& percentOfTotalValue,
        const std::optional<glm::vec3>& positionValue,
        const std::optional<std::string>& dimensionValue
    ) :
        displayName(displayNameValue),
        entity(entityValue),
        timeInNS(timeInNSValue),
        percentOfTotal(percentOfTotalValue),
        position(positionValue),
        dimension(dimensionValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        displayName = stream.readString().value_or("");
        entity = stream.readString().value_or("");
        timeInNS = stream.readUnsignedLong().value_or(0);
        percentOfTotal = stream.readUnsignedByte().value_or(0);
        position = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        dimension = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(displayName);
		stream.writeString(entity);
		stream.writeUnsignedLong(timeInNS);
		stream.writeUnsignedByte(percentOfTotal);
		stream.writeOptional(position, [&](const auto& value) {
		            stream.writeVec3(value);
		        });
		stream.writeOptional(dimension, [&](const auto& value) {
		            stream.writeString(value);
		        });
    }
};
