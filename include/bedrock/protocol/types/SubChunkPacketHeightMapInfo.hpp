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

class SubChunkPacketHeightMapInfo {
public:
    static constexpr int64_t ROW_LENGTH = 16;

    static constexpr auto TOTAL_LENGTH = (ROW_LENGTH + 2);

    std::vector<int32_t> heights;

    SubChunkPacketHeightMapInfo() = default;
    explicit SubChunkPacketHeightMapInfo(const std::vector<int32_t>& heightsValue) : heights(heightsValue) {
		if ((heightsValue.size() != TOTAL_LENGTH)) {
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
