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
#include <bedrock/protocol/types/WorldPosition.hpp>

class LocatorBarWaypoint {
public:
    uint32_t updateFlag;
    std::optional<bool> visible;
    std::optional<std::shared_ptr<WorldPosition>> worldPosition;
    std::optional<int32_t> textureId;
    std::optional<uint32_t> color;
    std::optional<bool> clientPositionAuthority;
    std::optional<int32_t> actorUniqueId;

    LocatorBarWaypoint() = default;
    explicit LocatorBarWaypoint(
        const uint32_t& updateFlagValue,
        const std::optional<bool>& visibleValue,
        const std::optional<std::shared_ptr<WorldPosition>>& worldPositionValue,
        const std::optional<int32_t>& textureIdValue,
        const std::optional<uint32_t>& colorValue,
        const std::optional<bool>& clientPositionAuthorityValue,
        const std::optional<int32_t>& actorUniqueIdValue
    ) :
        updateFlag(updateFlagValue),
        visible(visibleValue),
        worldPosition(worldPositionValue),
        textureId(textureIdValue),
        color(colorValue),
        clientPositionAuthority(clientPositionAuthorityValue),
        actorUniqueId(actorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        updateFlag = stream.readUnsignedInt().value_or(0);
        visible = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        worldPosition = stream.readOptional([&]() {
            return stream.readObject<WorldPosition>();
        });
        textureId = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
        color = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        clientPositionAuthority = stream.readOptional([&]() {
            return stream.readBool().value_or(false);
        });
        actorUniqueId = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUnsignedInt(updateFlag);
		stream.writeOptional(visible, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(worldPosition, [&](const auto& v) {
			v->write(stream);
		});
		stream.writeOptional(textureId, [&](const auto& value) {
			stream.writeUnsignedInt(value);
		});
		stream.writeOptional(color, [&](const auto& v) {
			stream.writeUnsignedInt(static_cast<uint32_t>(v));
		});
		stream.writeOptional(clientPositionAuthority, [&](const auto& value) {
			stream.writeBool(value);
		});
		stream.writeOptional(actorUniqueId, [&](const auto& value) {
			stream.writeActorUniqueID(value);
		});
    }
};
