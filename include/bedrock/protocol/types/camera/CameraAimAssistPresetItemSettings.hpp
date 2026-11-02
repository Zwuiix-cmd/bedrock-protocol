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
