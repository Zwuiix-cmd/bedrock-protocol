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

class TrimMaterial {
public:
    std::string materialId;
    std::string color;
    std::string itemId;

    TrimMaterial() = default;
    explicit TrimMaterial(const std::string& materialIdValue, const std::string& colorValue, const std::string& itemIdValue) : materialId(materialIdValue), color(colorValue), itemId(itemIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        materialId = stream.readString().value_or("");
        color = stream.readString().value_or("");
        itemId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(materialId);
		stream.writeString(color);
		stream.writeString(itemId);
    }
};
