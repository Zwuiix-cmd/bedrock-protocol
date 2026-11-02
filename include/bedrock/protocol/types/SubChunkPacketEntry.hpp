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
#include <bedrock/protocol/types/SubChunkPacketHeightMapInfo.hpp>
#include <bedrock/protocol/types/SubChunkPositionOffset.hpp>
#include <bedrock/protocol/types/SubChunkPacketHeightMapInfo.hpp>
#include <bedrock/protocol/types/SubChunkPositionOffset.hpp>

class SubChunkPacketEntry {
public:
    std::shared_ptr<SubChunkPositionOffset> offset;
    uint8_t requestResult;
    std::optional<std::string> terrainData;
    uint8_t heightMapType;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> heightMapData;
    uint8_t renderHeightMapType;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> renderHeightMapData;
    std::optional<int32_t> usedBlobHash;

    SubChunkPacketEntry() = default;
    explicit SubChunkPacketEntry(
        const std::shared_ptr<SubChunkPositionOffset>& offsetValue,
        const uint8_t& requestResultValue,
        const std::optional<std::string>& terrainDataValue,
        const uint8_t& heightMapTypeValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& heightMapDataValue,
        const uint8_t& renderHeightMapTypeValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& renderHeightMapDataValue,
        const std::optional<int32_t>& usedBlobHashValue
    ) :
        offset(offsetValue),
        requestResult(requestResultValue),
        terrainData(terrainDataValue),
        heightMapType(heightMapTypeValue),
        heightMapData(heightMapDataValue),
        renderHeightMapType(renderHeightMapTypeValue),
        renderHeightMapData(renderHeightMapDataValue),
        usedBlobHash(usedBlobHashValue) {
		if (((heightMapTypeValue == static_cast<std::underlying_type_t<SubChunkPacketHeightMapType>>(SubChunkPacketHeightMapType::Data)) && (heightMapDataValue == nullptr))) {
			throw std::exception{};
		}
		if (((renderHeightMapTypeValue == static_cast<std::underlying_type_t<SubChunkPacketHeightMapType>>(SubChunkPacketHeightMapType::Data)) && (renderHeightMapDataValue == nullptr))) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        requestResult = stream.readUnsignedByte().value_or(0);
        heightMapType = stream.readUnsignedByte().value_or(0);
        renderHeightMapType = stream.readUnsignedByte().value_or(0);
        offset = ([&]() {
    auto value = std::make_shared<SubChunkPositionOffset>();
    value->read(stream);
    return value;
}());
        terrainData = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
        heightMapData = stream.readOptional([&]() {
			return ([&]() {
			auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
			value->read(stream);
			return value;
			}());
		});
        renderHeightMapData = stream.readOptional([&]() {
			return ([&]() {
			auto value = std::make_shared<SubChunkPacketHeightMapInfo>();
			value->read(stream);
			return value;
			}());
		});
        usedBlobHash = stream.readOptional([&]() {
			return stream.readInt().value_or(0);
		});
    }

    virtual void write(BinaryStream& stream) const {
		offset->write(stream);
		stream.writeUnsignedByte(requestResult);
		stream.writeOptional(terrainData, [&](const auto& value) {
					stream.writeString(value);
				});
		stream.writeUnsignedByte(heightMapType);
		stream.writeOptional(heightMapData, [&](const auto& v) {
					v->write(stream);
				});
		stream.writeUnsignedByte(renderHeightMapType);
		stream.writeOptional(renderHeightMapData, [&](const auto& v) {
					v->write(stream);
				});
		stream.writeOptional(usedBlobHash, [&](const auto& value) {
					stream.writeUnsignedLong(value);
				});
    }
};
