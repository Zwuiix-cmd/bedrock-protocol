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


class MapImage {
public:
    enum class Max : uint8_t {
        Height = 128,
        Width = 128,
    };

    int32_t width;
    int32_t height;
    std::vector<std::vector<uint32_t>> pixels;
    mutable std::optional<std::string> encodedPixelCache;

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
		if (width > magic_enum::enum_integer(Max::Width)) {
			throw std::exception{};
		}
		if (height > magic_enum::enum_integer(Max::Height)) {
			throw std::exception{};
		}
		auto pixels = std::vector<std::vector<uint32_t>>();
		for (int32_t y = 0; (y < height) && !stream.hasOverflowed(); ++y) {
			auto row = std::vector<uint32_t>();
			for (int32_t x = 0; (x < width) && !stream.hasOverflowed(); ++x) {
				row.push_back(static_cast<uint32_t>(BinaryStream::flipIntEndianness(stream.readUnsignedVarInt().value_or(0))));
			}
			pixels.push_back(row);
		}
		return std::make_shared<MapImage>(pixels);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        pixels = stream.readList([&]() {
            return stream.readList([&]() {
                return stream.readUnsignedInt().value_or(0);
            });
        });
        width = stream.readInt().value_or(0);
        height = stream.readInt().value_or(0);
        encodedPixelCache = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }

    virtual void write(BinaryStream& stream) const {
		if (!encodedPixelCache) {
			auto serializer = BinaryStream();
			for (std::size_t y = 0; (y < height); ++y) {
				for (std::size_t x = 0; (x < width); ++x) {
					serializer.writeUnsignedVarInt(BinaryStream::flipIntEndianness(static_cast<uint32_t>(pixels[y][x])));
				}
			}
			encodedPixelCache = serializer.getBuffer();
		}
		stream.writeByteArray(*(encodedPixelCache));
    }
};
