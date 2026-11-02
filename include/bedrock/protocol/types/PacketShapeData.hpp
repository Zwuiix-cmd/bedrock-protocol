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
#include <bedrock/protocol/types/ScriptDebugShapeType.hpp>

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
    std::optional<int32_t> attachedToEntityId;

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
        const std::optional<int32_t>& attachedToEntityIdValue
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
        text = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        boxBound = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        lineEndLocation = stream.readOptional([&]() {
            return stream.readVec3().value_or(glm::vec3(0.0f));
        });
        segments = stream.readOptional([&]() {
            return stream.readUnsignedByte().value_or(0);
        });
        type = stream.readOptional([&]() {
            return magic_enum::enum_cast<ScriptDebugShapeType>(
                stream.readVarInt().value_or(0)
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
        arrowHeadLength = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        arrowHeadRadius = stream.readOptional([&]() {
            return stream.readFloat().value_or(0.0f);
        });
        dimensionId = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
        attachedToEntityId = stream.readOptional([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedVarLong(networkId);
        stream.writeOptional(text, [&](const auto& optionalValue) {
            stream.writeString(optionalValue);
        });
        stream.writeOptional(boxBound, [&](const auto& optionalValue) {
            stream.writeVec3(optionalValue);
        });
        stream.writeOptional(lineEndLocation, [&](const auto& optionalValue) {
            stream.writeVec3(optionalValue);
        });
        stream.writeOptional(segments, [&](const auto& optionalValue) {
            stream.writeUnsignedByte(optionalValue);
        });
        stream.writeOptional(magic_enum::enum_integer(type), [&](const auto& optionalValue) {
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
        stream.writeOptional(arrowHeadLength, [&](const auto& optionalValue) {
            stream.writeFloat(optionalValue);
        });
        stream.writeOptional(arrowHeadRadius, [&](const auto& optionalValue) {
            stream.writeFloat(optionalValue);
        });
        stream.writeOptional(dimensionId, [&](const auto& optionalValue) {
            stream.writeInt(optionalValue);
        });
        stream.writeOptional(attachedToEntityId, [&](const auto& optionalValue) {
            stream.writeInt(optionalValue);
        });
    }
};
