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

class SkinImage {
public:
    uint32_t height;
    uint32_t width;
    std::string data;

    SkinImage() = default;
    explicit SkinImage(const uint32_t& heightValue, const uint32_t& widthValue, const std::string& dataValue) : height(heightValue), width(widthValue), data(dataValue) {
		std::remove_cvref_t<decltype(((heightValue * widthValue) * 4))> expected{};
		std::remove_cvref_t<decltype(dataValue.size())> actual{};
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
