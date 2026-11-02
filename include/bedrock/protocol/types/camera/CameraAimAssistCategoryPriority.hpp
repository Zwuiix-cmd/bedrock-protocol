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

class CameraAimAssistCategoryPriority {
public:
    std::string identifier;
    int32_t priority;

    CameraAimAssistCategoryPriority() = default;
    explicit CameraAimAssistCategoryPriority(const std::string& identifierValue, const int32_t& priorityValue) : identifier(identifierValue), priority(priorityValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        identifier = stream.readString().value_or("");
        priority = stream.readInt().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(identifier);
		stream.writeInt(priority);
    }
};
