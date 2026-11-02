#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class CameraAimAssistPresetItemSettings {
public:
    std::string itemIdentifier;
    std::string categoryName;

    CameraAimAssistPresetItemSettings() = default;
    explicit CameraAimAssistPresetItemSettings(const std::string& itemIdentifierValue, const std::string& categoryNameValue) : itemIdentifier(itemIdentifierValue), categoryName(categoryNameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        itemIdentifier = stream.readString().value_or("");
        categoryName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(itemIdentifier);
		stream.writeString(categoryName);
    }
};

} // namespace bedrock::protocol
