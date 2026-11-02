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

class CameraTargetInstruction {
public:
    std::optional<glm::vec3> targetCenterOffset;
    int64_t actorUniqueId;

    CameraTargetInstruction() = default;
    explicit CameraTargetInstruction(const std::optional<glm::vec3>& targetCenterOffsetValue, const int64_t& actorUniqueIdValue) : targetCenterOffset(targetCenterOffsetValue), actorUniqueId(actorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        actorUniqueId = stream.readLong().value_or(0);
        targetCenterOffset = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeOptional(targetCenterOffset, [&](const auto& value) {
			stream.writeVec3(value);
		});
		stream.writeLong(actorUniqueId);
    }
};
