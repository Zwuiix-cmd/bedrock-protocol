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

class TrimPattern {
public:
    std::string itemId;
    std::string patternId;

    TrimPattern() = default;
    explicit TrimPattern(const std::string& itemIdValue, const std::string& patternIdValue) : itemId(itemIdValue), patternId(patternIdValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        itemId = stream.readString().value_or("");
        patternId = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(itemId);
		stream.writeString(patternId);
    }
};
