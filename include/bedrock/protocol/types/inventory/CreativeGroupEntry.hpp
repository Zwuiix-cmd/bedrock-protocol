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
#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>

class CreativeGroupEntry {
public:
    uint8_t categoryId;
    std::string categoryName;
    NetworkItemDescriptor icon;

    CreativeGroupEntry() = default;
    explicit CreativeGroupEntry(
        const uint8_t& categoryIdValue,
        const std::string& categoryNameValue,
        const NetworkItemDescriptor& iconValue
    ) :
        categoryId(categoryIdValue),
        categoryName(categoryNameValue),
        icon(iconValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        categoryId = stream.readUnsignedByte().value_or(0);
        categoryName = stream.readString().value_or("");
        icon.read(stream);
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeUnsignedByte(categoryId);
        stream.writeString(categoryName);
        icon.write(stream);
    }
};
