#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <optional>


class CameraTargetInstruction {
public:
    std::optional<glm::vec3> targetCenterOffset;
    int64_t actorUniqueId;

    CameraTargetInstruction() = default;
    explicit CameraTargetInstruction(const std::optional<glm::vec3>& targetCenterOffsetValue, const int64_t& actorUniqueIdValue) : targetCenterOffset(targetCenterOffsetValue), actorUniqueId(actorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        targetCenterOffset = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        actorUniqueId = stream.readLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(targetCenterOffset, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeLong(actorUniqueId);
    }
};
