#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <exception>
#include <memory>
#include <vector>

namespace bedrock::protocol {


class SubChunkPacketHeightMapInfo {
public:
    std::vector<int8_t> heights;

    SubChunkPacketHeightMapInfo() = default;
    explicit SubChunkPacketHeightMapInfo(const std::vector<int8_t>& heightsValue) : heights(heightsValue) {
		if (heightsValue.size() != 256) {
			throw std::exception{};
		}
    }

    static std::shared_ptr<SubChunkPacketHeightMapInfo> create(ReadOnlyBinaryStream& stream) {
        auto result = std::make_shared<SubChunkPacketHeightMapInfo>();
        result->heights = stream.readList([&]() {
            return stream.readByte().value_or(0);
        });
        return result;
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeList(heights, [&](const auto& item) {
            stream.writeByte(item);
        });
    }
};

} // namespace bedrock::protocol
