#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class EducationSettingsExternalLinkSettings {
public:
    std::string url;
    std::string displayName;

    EducationSettingsExternalLinkSettings() = default;
    explicit EducationSettingsExternalLinkSettings(const std::string& urlValue, const std::string& displayNameValue) : url(urlValue), displayName(displayNameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        url = stream.readString().value_or("");
        displayName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(url);
		stream.writeString(displayName);
    }
};

} // namespace bedrock::protocol
