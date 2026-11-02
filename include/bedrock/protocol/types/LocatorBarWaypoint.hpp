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
#include <bedrock/protocol/types/WorldPosition.hpp>
#include <bedrock/protocol/types/WorldPosition.hpp>

class LocatorBarWaypoint {
public:
    uint32_t updateFlag;
    std::optional<bool> visible;
    std::optional<std::shared_ptr<WorldPosition>> worldPosition;
    std::optional<std::string> texturePath;
    std::optional<glm::vec2> iconSize;
    std::optional<uint32_t> color;
    std::optional<bool> clientPositionAuthority;
    std::optional<int32_t> actorUniqueId;

    LocatorBarWaypoint() = default;
    explicit LocatorBarWaypoint(
        const uint32_t& updateFlagValue,
        const std::optional<bool>& visibleValue,
        const std::optional<std::shared_ptr<WorldPosition>>& worldPositionValue,
        const std::optional<std::string>& texturePathValue,
        const std::optional<glm::vec2>& iconSizeValue,
        const std::optional<uint32_t>& colorValue,
        const std::optional<bool>& clientPositionAuthorityValue,
        const std::optional<int32_t>& actorUniqueIdValue
    ) :
        updateFlag(updateFlagValue),
        visible(visibleValue),
        worldPosition(worldPositionValue),
        texturePath(texturePathValue),
        iconSize(iconSizeValue),
        color(colorValue),
        clientPositionAuthority(clientPositionAuthorityValue),
        actorUniqueId(actorUniqueIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        updateFlag = stream.readUnsignedInt().value_or(0);
        visible = stream.readOptional([&]() {
			return stream.readBool().value_or(false);
		});
        worldPosition = stream.readOptional([&]() {
			return ([&]() {
			auto value = std::make_shared<WorldPosition>();
			value->read(stream);
			return value;
			}());
		});
        texturePath = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        iconSize = stream.readOptional([&]() {
			return stream.readVec2().value_or(glm::vec2(0.0f));
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
        stream.writeOptional(visible, [&](const auto& optionalValue) {
			stream.writeBool(optionalValue);
		});
        stream.writeOptional(worldPosition, [&](const auto& optionalValue) {
			if (optionalValue) optionalValue->write(stream);
		});
        stream.writeOptional(texturePath, [&](const auto& optionalValue) {
			stream.writeString(optionalValue);
		});
        stream.writeOptional(iconSize, [&](const auto& optionalValue) {
			stream.writeVec2(optionalValue);
		});
        stream.writeOptional(color, [&](const auto& optionalValue) {
			stream.writeUnsignedInt(optionalValue);
		});
        stream.writeOptional(clientPositionAuthority, [&](const auto& optionalValue) {
			stream.writeBool(optionalValue);
		});
        stream.writeOptional(actorUniqueId, [&](const auto& optionalValue) {
			stream.writeInt(optionalValue);
		});
    }
};
