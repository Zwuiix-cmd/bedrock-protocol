<?php

declare(strict_types=1);

return [
    [<<<'BEFORE'
    virtual void read(ReadOnlyBinaryStream& stream) {
        pixels = stream.readList([&]() {
            return stream.readIntBE().value_or(0);
        });
        width = stream.readInt().value_or(0);
        height = stream.readInt().value_or(0);
        encodedPixelCache = stream.readOptional([&]() {
            return stream.readString().value_or("");
        });
    }
BEFORE,
<<<'AFTER'
    virtual void read(ReadOnlyBinaryStream& stream) {
        *this = *create(stream, height, width);
    }
AFTER],
    [<<<'BEFORE'
    virtual void write(BinaryStream& stream) const {
		if (!encodedPixelCache) {
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
BEFORE,
<<<'AFTER'
    virtual void write(BinaryStream& stream) const {
        for (const auto& row : pixels) {
            for (const auto pixel : row) {
                stream.writeIntBE(static_cast<int32_t>(pixel));
            }
        }
    }
AFTER],
    [<<<'BEFORE'
    mutable std::optional<std::string> encodedPixelCache;

BEFORE,
<<<'AFTER'

AFTER],
    [<<<'BEFORE'
if (width > magic_enum::enum_integer(Max::Width))
BEFORE,
<<<'AFTER'
if (width < 0 || width > magic_enum::enum_integer(Max::Width))
AFTER],
    [<<<'BEFORE'
if (height > magic_enum::enum_integer(Max::Height))
BEFORE,
<<<'AFTER'
if (height < 1 || height > magic_enum::enum_integer(Max::Height))
AFTER],
];
