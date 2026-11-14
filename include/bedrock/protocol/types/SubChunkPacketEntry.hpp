#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <string>

#include <bedrock/protocol/types/SubChunkPacketHeightMapInfo.hpp>
#include <bedrock/protocol/types/SubChunkPositionOffset.hpp>
namespace bedrock::protocol {


class SubChunkPacketEntry {
public:
    std::shared_ptr<SubChunkPositionOffset> offset;
    uint8_t requestResult;
    std::optional<std::string> terrainData;
    uint8_t heightMapType;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> heightMapData;
    uint8_t renderHeightMapType;
    std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>> renderHeightMapData;
    std::optional<uint64_t> usedBlobHash;

    SubChunkPacketEntry() = default;
    explicit SubChunkPacketEntry(
        const std::shared_ptr<SubChunkPositionOffset>& offsetValue,
        const uint8_t& requestResultValue,
        const std::optional<std::string>& terrainDataValue,
        const uint8_t& heightMapTypeValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& heightMapDataValue,
        const uint8_t& renderHeightMapTypeValue,
        const std::optional<std::shared_ptr<SubChunkPacketHeightMapInfo>>& renderHeightMapDataValue,
        const std::optional<uint64_t>& usedBlobHashValue
    ) :
        offset(offsetValue),
        requestResult(requestResultValue),
        terrainData(terrainDataValue),
        heightMapType(heightMapTypeValue),
        heightMapData(heightMapDataValue),
        renderHeightMapType(renderHeightMapTypeValue),
        renderHeightMapData(renderHeightMapDataValue),
        usedBlobHash(usedBlobHashValue) {
		if ((heightMapTypeValue == magic_enum::enum_integer(SubChunkPacketHeightMapType::Data)) && (heightMapDataValue == nullptr)) {
			throw std::exception{};
		}
		if ((renderHeightMapTypeValue == magic_enum::enum_integer(SubChunkPacketHeightMapType::Data)) && (renderHeightMapDataValue == nullptr)) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        offset = std::make_shared<SubChunkPositionOffset>();
        offset->read(stream);
        requestResult = stream.readUnsignedByte().value_or(0);
        terrainData = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
        heightMapType = stream.readUnsignedByte().value_or(0);
        heightMapData = stream.readOptional([&]() {
            return stream.readObject<SubChunkPacketHeightMapInfo>();
        });
        renderHeightMapType = stream.readUnsignedByte().value_or(0);
        renderHeightMapData = stream.readOptional([&]() {
            return stream.readObject<SubChunkPacketHeightMapInfo>();
        });
        usedBlobHash = stream.readOptional([&]() {
            return stream.readUnsignedLong().value_or(0);
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

} // namespace bedrock::protocol
