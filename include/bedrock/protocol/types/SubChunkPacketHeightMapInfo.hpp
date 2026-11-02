#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>
#include <vector>


class SubChunkPacketHeightMapInfo {
public:
    std::vector<int32_t> heights;

    SubChunkPacketHeightMapInfo() = default;
    explicit SubChunkPacketHeightMapInfo(const std::vector<int32_t>& heightsValue) : heights(heightsValue) {
		if (heightsValue.size() != 256) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        heights = stream.readList([&]() {
            return stream.readInt().value_or(0);
        });
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(heights, [&](const auto& item) {
            stream.writeInt(item);
        });
    }
};
