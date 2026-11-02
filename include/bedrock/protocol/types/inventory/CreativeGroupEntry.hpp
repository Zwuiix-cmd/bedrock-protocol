#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>

#include <bedrock/protocol/types/inventory/NetworkItemDescriptor.hpp>

class CreativeGroupEntry {
public:
    int32_t categoryId;
    std::string categoryName;
    NetworkItemDescriptor icon;

    CreativeGroupEntry() = default;
    explicit CreativeGroupEntry(
        const int32_t& categoryIdValue,
        const std::string& categoryNameValue,
        const NetworkItemDescriptor& iconValue
    ) :
        categoryId(categoryIdValue),
        categoryName(categoryNameValue),
        icon(iconValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        categoryId = stream.readInt().value_or(0);
        categoryName = stream.readString().value_or("");
        icon = stream.readValue<NetworkItemDescriptor>();
    }

    virtual void write(BinaryStream& stream) const {
        stream.writeInt(categoryId);
        stream.writeString(categoryName);
        icon.write(stream);
    }
};
