#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <string>

namespace bedrock::protocol {


class SkinImage {
public:
    uint32_t height;
    uint32_t width;
    std::string data;

    SkinImage() = default;
    explicit SkinImage(const uint32_t& heightValue, const uint32_t& widthValue, const std::string& dataValue) : height(heightValue), width(widthValue), data(dataValue) {
		int64_t expected{};
		std::size_t actual{};
		if ((heightValue < 0) || (widthValue < 0)) {
			throw std::exception{};
		}
		if ((expected = ((heightValue * widthValue) * 4)) != (actual = dataValue.size())) {
			throw std::exception{};
		}
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        height = stream.readUnsignedInt().value_or(0);
        width = stream.readUnsignedInt().value_or(0);
        data = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedInt(height);
        stream.writeUnsignedInt(width);
        stream.writeString(data);
    }
};

} // namespace bedrock::protocol
