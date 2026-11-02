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
#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeType.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapePayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeType.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeArrowPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeBoxPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeCircleOrSpherePayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeConePayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeCylinderPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeEllipsoidPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeLinePayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapePyramidPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeTextPayload.hpp>
#include <bedrock/protocol/types/shape/PrimitiveShapeType.hpp>

class PacketShapeData {
public:
    uint64_t networkId;
    std::optional<PrimitiveShapeType> type;
    std::optional<glm::vec3> location;
    std::optional<float> scale;
    std::optional<glm::vec3> rotation;
    std::optional<float> totalTimeLeft;
    std::optional<float> maximumRenderDistance;
    std::optional<uint32_t> color;
    std::optional<int32_t> dimensionId;
    std::optional<int32_t> attachedToEntityId;
    std::optional<std::shared_ptr<PrimitiveShapePayload>> payload;

    PacketShapeData() = default;
    explicit PacketShapeData(
        const uint64_t& networkIdValue,
        const std::optional<PrimitiveShapeType>& typeValue,
        const std::optional<glm::vec3>& locationValue,
        const std::optional<float>& scaleValue,
        const std::optional<glm::vec3>& rotationValue,
        const std::optional<float>& totalTimeLeftValue,
        const std::optional<float>& maximumRenderDistanceValue,
        const std::optional<uint32_t>& colorValue,
        const std::optional<int32_t>& dimensionIdValue,
        const std::optional<int32_t>& attachedToEntityIdValue,
        const std::optional<std::shared_ptr<PrimitiveShapePayload>>& payloadValue
    ) :
        networkId(networkIdValue),
        type(typeValue),
        location(locationValue),
        scale(scaleValue),
        rotation(rotationValue),
        totalTimeLeft(totalTimeLeftValue),
        maximumRenderDistance(maximumRenderDistanceValue),
        color(colorValue),
        dimensionId(dimensionIdValue),
        attachedToEntityId(attachedToEntityIdValue),
        payload(payloadValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        networkId = stream.readUnsignedVarLong().value_or(0);
        type = stream.readOptional([&]() {
	return magic_enum::enum_cast<PrimitiveShapeType>(stream.readVarInt().value_or(0)).value_or(magic_enum::enum_value<PrimitiveShapeType>(0));
});
        location = stream.readOptional([&]() {
	return stream.readVec3().value_or(glm::vec3(0.0f));
});
        scale = stream.readOptional([&]() {
	return stream.readFloat().value_or(0.0f);
});
        rotation = stream.readOptional([&]() {
	return stream.readVec3().value_or(glm::vec3(0.0f));
});
        totalTimeLeft = stream.readOptional([&]() {
	return stream.readFloat().value_or(0.0f);
});
        maximumRenderDistance = stream.readOptional([&]() {
	return stream.readFloat().value_or(0.0f);
});
        color = stream.readOptional([&]() {
	return stream.readUnsignedInt().value_or(0);
});
        dimensionId = stream.readOptional([&]() {
	return stream.readInt().value_or(0);
});
        attachedToEntityId = stream.readOptional([&]() {
	return stream.readInt().value_or(0);
});
        payload = stream.readOptional([&]() {
	return PrimitiveShapePayload::decode(stream);
});
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarLong(networkId);
        stream.writeOptional(type, [&](const auto& optionalValue) {
	stream.writeVarInt(magic_enum::enum_integer(optionalValue));
});
        stream.writeOptional(location, [&](const auto& optionalValue) {
	stream.writeVec3(optionalValue);
});
        stream.writeOptional(scale, [&](const auto& optionalValue) {
	stream.writeFloat(optionalValue);
});
        stream.writeOptional(rotation, [&](const auto& optionalValue) {
	stream.writeVec3(optionalValue);
});
        stream.writeOptional(totalTimeLeft, [&](const auto& optionalValue) {
	stream.writeFloat(optionalValue);
});
        stream.writeOptional(maximumRenderDistance, [&](const auto& optionalValue) {
	stream.writeFloat(optionalValue);
});
        stream.writeOptional(color, [&](const auto& optionalValue) {
	stream.writeUnsignedInt(optionalValue);
});
        stream.writeOptional(dimensionId, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        stream.writeOptional(attachedToEntityId, [&](const auto& optionalValue) {
	stream.writeInt(optionalValue);
});
        stream.writeOptional(payload, [&](const auto& optionalValue) {
	if (optionalValue) optionalValue->write(stream);
});
    }
};
