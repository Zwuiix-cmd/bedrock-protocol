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

class EducationUriResource {
public:
    std::string buttonName;
    std::string linkUri;

    EducationUriResource() = default;
    explicit EducationUriResource(const std::string& buttonNameValue, const std::string& linkUriValue) : buttonName(buttonNameValue), linkUri(linkUriValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        buttonName = stream.readString().value_or("");
        linkUri = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(buttonName);
		stream.writeString(linkUri);
    }
};
