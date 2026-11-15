#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace bedrock::protocol {


class MapImage {
public:
    enum class Max : uint8_t {
        Height = 128,
        Width = 128,
    };

    int32_t width;
    int32_t height;
    std::vector<std::vector<uint32_t>> pixels;


    MapImage() = default;
    explicit MapImage(const std::vector<std::vector<uint32_t>>& pixelsValue) : pixels(pixelsValue) {
		std::optional<std::size_t> rowLength;
		for (const auto& row : pixelsValue) {
			if (!rowLength) {
				rowLength = row.size();
			} else if (row.size() != rowLength.value()) {
				throw std::exception{};
			}
		}
		if (!rowLength) {
			throw std::exception{};
		}
		if (rowLength.value() > magic_enum::enum_integer(Max::Width)) {
			throw std::exception{};
		}
		if (pixelsValue.size() > magic_enum::enum_integer(Max::Height)) {
			throw std::exception{};
		}
		height = pixelsValue.size();
		width = rowLength.value();
    }

    static std::shared_ptr<MapImage> create(ReadOnlyBinaryStream& stream, const int32_t& height, const int32_t& width) {
		if (width < 0 || width > magic_enum::enum_integer(Max::Width)) {
			throw std::exception{};
		}
		if (height < 1 || height > magic_enum::enum_integer(Max::Height)) {
			throw std::exception{};
		}
		auto pixels = std::vector<std::vector<uint32_t>>();
		for (int32_t y = 0; (y < height) && !stream.hasOverflowed(); ++y) {
			auto row = std::vector<uint32_t>();
			for (int32_t x = 0; (x < width) && !stream.hasOverflowed(); ++x) {
				row.push_back(static_cast<uint32_t>(stream.readIntBE().value_or(0)));
			}
			pixels.push_back(row);
		}
		return std::make_shared<MapImage>(pixels);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        *this = *create(stream, height, width);
    }

    virtual void write(BinaryStream& stream) const {
        for (const auto& row : pixels) {
            for (const auto pixel : row) {
                stream.writeIntBE(static_cast<int32_t>(pixel));
            }
        }
    }
};

} // namespace bedrock::protocol
