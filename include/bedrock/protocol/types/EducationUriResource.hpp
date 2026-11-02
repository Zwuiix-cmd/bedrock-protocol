#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>


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
