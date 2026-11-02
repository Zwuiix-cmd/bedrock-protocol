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

class SystemCategory {
public:
    std::string categoryName;
    uint64_t systemIndex;

    SystemCategory() = default;
    explicit SystemCategory(const std::string& categoryNameValue, const uint64_t& systemIndexValue) : categoryName(categoryNameValue), systemIndex(systemIndexValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        categoryName = stream.readString().value_or("");
        systemIndex = stream.readUnsignedLong().value_or(0);
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(categoryName);
		stream.writeUnsignedLong(systemIndex);
    }
};
