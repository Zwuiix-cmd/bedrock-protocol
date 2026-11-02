#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <optional>
#include <string>

#include <bedrock/protocol/types/ScriptDebugShapeType.hpp>
namespace bedrock::protocol {


class PacketShapeData {
public:
    uint64_t networkId;
    std::optional<ScriptDebugShapeType> type;
    std::optional<glm::vec3> location;
    std::optional<float> scale;
    std::optional<glm::vec3> rotation;
    std::optional<float> totalTimeLeft;
    std::optional<uint32_t> color;
    std::optional<std::string> text;
    std::optional<glm::vec3> boxBound;
    std::optional<glm::vec3> lineEndLocation;
    std::optional<float> arrowHeadLength;
    std::optional<float> arrowHeadRadius;
    std::optional<uint8_t> segments;
    std::optional<int32_t> dimensionId;
    std::optional<uint64_t> attachedToEntityId;

    PacketShapeData() = default;
    explicit PacketShapeData(
        const uint64_t& networkIdValue,
        const std::optional<ScriptDebugShapeType>& typeValue,
        const std::optional<glm::vec3>& locationValue,
        const std::optional<float>& scaleValue,
        const std::optional<glm::vec3>& rotationValue,
        const std::optional<float>& totalTimeLeftValue,
        const std::optional<uint32_t>& colorValue,
        const std::optional<std::string>& textValue,
        const std::optional<glm::vec3>& boxBoundValue,
        const std::optional<glm::vec3>& lineEndLocationValue,
        const std::optional<float>& arrowHeadLengthValue,
        const std::optional<float>& arrowHeadRadiusValue,
        const std::optional<uint8_t>& segmentsValue,
        const std::optional<int32_t>& dimensionIdValue,
        const std::optional<uint64_t>& attachedToEntityIdValue
    ) :
        networkId(networkIdValue),
        type(typeValue),
        location(locationValue),
        scale(scaleValue),
        rotation(rotationValue),
        totalTimeLeft(totalTimeLeftValue),
        color(colorValue),
        text(textValue),
        boxBound(boxBoundValue),
        lineEndLocation(lineEndLocationValue),
        arrowHeadLength(arrowHeadLengthValue),
        arrowHeadRadius(arrowHeadRadiusValue),
        segments(segmentsValue),
        dimensionId(dimensionIdValue),
        attachedToEntityId(attachedToEntityIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        networkId = stream.readUnsignedVarLong().value_or(0);
        type = stream.readOptional([&]() {
            return magic_enum::enum_cast<ScriptDebugShapeType>(
            stream.readUnsignedByte().value_or(0)
        ).value_or(ScriptDebugShapeType::Line);
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
        color = stream.readOptional([&]() {
            return stream.readUnsignedInt().value_or(0);
        });
        text = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        boxBound = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        lineEndLocation = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        arrowHeadLength = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        arrowHeadRadius = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        segments = stream.readOptional([&]() {
            return stream.readUnsignedByte().value_or(0);
        });
        dimensionId = stream.readOptional([&]() {
            return stream.readVarInt().value_or(0);
        });
        attachedToEntityId = stream.readOptional([&]() {
            return stream.readActorRuntimeID().value_or(0);
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
        stream.writeOptional(color, [&](const auto& optionalValue) {
            stream.writeUnsignedInt(optionalValue);
        });
        stream.writeOptional(text, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
        stream.writeOptional(boxBound, [&](const auto& optionalValue) {
            stream.writeVec3(optionalValue);
        });
        stream.writeOptional(lineEndLocation, [&](const auto& optionalValue) {
            stream.writeVec3(optionalValue);
        });
        stream.writeOptional(arrowHeadLength, [&](const auto& optionalValue) {
            stream.writeFloat(optionalValue);
        });
        stream.writeOptional(arrowHeadRadius, [&](const auto& optionalValue) {
            stream.writeFloat(optionalValue);
        });
        stream.writeOptional(segments, [&](const auto& optionalValue) {
            stream.writeUnsignedByte(optionalValue);
        });
        stream.writeOptional(dimensionId, [&](const auto& optionalValue) {
            stream.writeVarInt(optionalValue);
        });
        stream.writeOptional(attachedToEntityId, [&](const auto& optionalValue) {
            stream.writeActorRuntimeID(optionalValue);
        });
    }
};

} // namespace bedrock::protocol
