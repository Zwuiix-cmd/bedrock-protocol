#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

namespace bedrock::protocol {


class SubChunkPacketHeightMapInfo {
public:
    static constexpr int64_t ROW_LENGTH = 16;

    static constexpr auto TOTAL_LENGTH = (ROW_LENGTH + 2);

    std::vector<int8_t> heights;

    SubChunkPacketHeightMapInfo() = default;
    explicit SubChunkPacketHeightMapInfo(const std::vector<int8_t>& heightsValue) : heights(heightsValue) {
		if (heightsValue.size() != TOTAL_LENGTH) {
			throw std::exception{};
		}
    }

    static std::shared_ptr<SubChunkPacketHeightMapInfo> create(ReadOnlyBinaryStream& stream) {
		auto heights = std::vector<int8_t>();
		for (std::size_t i = 0; (i < TOTAL_LENGTH) && !stream.hasOverflowed(); ++i) {
			if ((i & (ROW_LENGTH - 1)) == 0) {
				auto rowLength = stream.readUnsignedVarInt().value_or(0);
				if (rowLength != ROW_LENGTH) {
					throw std::exception{};
				}
			}
			heights.push_back(stream.readByte().value_or(0));
		}
		return std::make_shared<SubChunkPacketHeightMapInfo>(heights);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
		for (std::size_t i = 0; const auto& iValue : heights) {
		    const auto& height = iValue;
			if ((i & (ROW_LENGTH - 1)) == 0) {
				stream.writeUnsignedVarInt(ROW_LENGTH);
			}
			stream.writeByte(height);
		    ++i;
		}
    }
};

} // namespace bedrock::protocol
