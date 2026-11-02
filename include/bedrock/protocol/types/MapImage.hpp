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

class MapImage {
public:
    static constexpr int64_t MAX_HEIGHT = 128;
    static constexpr int64_t MAX_WIDTH = 128;

    int32_t width;
    int32_t height;
    std::vector<std::vector<uint32_t>> pixels;
    mutable std::optional<std::string> encodedPixelCache;

    MapImage() = default;
    explicit MapImage(const std::vector<std::vector<uint32_t>>& pixelsValue) : pixels(pixelsValue) {
		std::optional<std::size_t> rowLength;
		for (const auto& row : pixelsValue) {
			if ((!rowLength)) {
				rowLength = row.size();
			} else if ((row.size() != rowLength.value())) {
				throw std::exception{};
			}
		}
		if ((!rowLength)) {
			throw std::exception{};
		}
		if ((rowLength.value() > MAX_WIDTH)) {
			throw std::exception{};
		}
		if ((pixelsValue.size() > MAX_HEIGHT)) {
			throw std::exception{};
		}
		height = pixelsValue.size();
		width = rowLength.value();
		pixels = pixelsValue;
    }

    static std::shared_ptr<MapImage> decode(ReadOnlyBinaryStream& stream, const int32_t& height, const int32_t& width) {
		if ((width > MAX_WIDTH)) {
			throw std::exception{};
		}
		if ((height > MAX_HEIGHT)) {
			throw std::exception{};
		}
		auto pixels = std::vector<std::vector<uint32_t>>();
		for (std::size_t y = 0; (y < height); ++y) {
			auto row = std::vector<uint32_t>();
			for (std::size_t x = 0; (x < width); ++x) {
				row.push_back(static_cast<uint32_t>(stream.readIntBE().value_or(0)));
			}
			pixels.push_back(row);
		}
		return std::make_shared<MapImage>(pixels);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        width = stream.readInt().value_or(0);
        height = stream.readInt().value_or(0);
        pixels = stream.readList([&]() {
			return stream.readList([&]() {
			return stream.readUnsignedInt().value_or(0);
			});
		});
        encodedPixelCache = stream.readOptional([&]() {
			return stream.readString().value_or("");
		});
    }

    virtual void write(BinaryStream& stream) const {
		if ((!encodedPixelCache)) {
			auto serializer = BinaryStream();
			for (std::size_t y = 0; (y < height); ++y) {
				for (std::size_t x = 0; (x < width); ++x) {
					serializer.writeIntBE(static_cast<uint32_t>(pixels[y][x]));
				}
			}
			encodedPixelCache = serializer.getBuffer();
		}
		stream.writeByteArray(*(encodedPixelCache));
    }
};
