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
#include <bedrock/protocol/types/SubChunkPacketHeightMapInfo.hpp>
#include <bedrock/protocol/types/SubChunkPositionOffset.hpp>

class SubChunkPacketEntryCommon {
public:
    std::shared_ptr<SubChunkPositionOffset> offset;
    uint8_t requestResult;
    std::string terrainData;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> heightMap;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> renderHeightMap;

    SubChunkPacketEntryCommon() = default;
    explicit SubChunkPacketEntryCommon(
        const std::shared_ptr<SubChunkPositionOffset>& offsetValue,
        const uint8_t& requestResultValue,
        const std::string& terrainDataValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& heightMapValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& renderHeightMapValue
    ) :
        offset(offsetValue),
        requestResult(requestResultValue),
        terrainData(terrainDataValue),
        heightMap(heightMapValue),
        renderHeightMap(renderHeightMapValue) {}

    static std::shared_ptr<SubChunkPacketEntryCommon> read(ReadOnlyBinaryStream& stream, const bool& cacheEnabled) {
        auto result = std::make_shared<SubChunkPacketEntryCommon>();
        result->requestResult = stream.readUnsignedByte().value_or(0);
        result->terrainData = stream.readString().value_or("");
        result->offset = std::make_shared<SubChunkPositionOffset>();
        result->offset->read(stream);
        result->heightMap = stream.readOptional([&]() {
            auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
            value->read(stream);
            return value;
        });
        result->renderHeightMap = stream.readOptional([&]() {
            auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
            value->read(stream);
            return value;
        });
        return result;
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        requestResult = stream.readUnsignedByte().value_or(0);
        terrainData = stream.readString().value_or("");
        offset = std::make_shared<SubChunkPositionOffset>();
        offset->read(stream);
        heightMap = stream.readOptional([&]() {
            auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
            value->read(stream);
            return value;
        });
        renderHeightMap = stream.readOptional([&]() {
            auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
            value->read(stream);
            return value;
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedByte(requestResult);
        stream.writeString(terrainData);
        offset->write(stream);
        stream.writeOptional(heightMap, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
        stream.writeOptional(renderHeightMap, [&](const auto& optionalValue) {
            optionalValue->write(stream);
        });
    }
};
