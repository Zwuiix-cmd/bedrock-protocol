#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

#include <bedrock/protocol/types/entity/MetadataProperty.hpp>
namespace bedrock::protocol {


class AudioContentRegistrationEntry {
public:
    std::string audioContentId;
    std::string sharedMetadata;
    std::string serverContent;
    std::string playbackContent;

    AudioContentRegistrationEntry() = default;
    explicit AudioContentRegistrationEntry(
        const std::string& audioContentIdValue,
        const std::string& sharedMetadataValue,
        const std::string& serverContentValue,
        const std::string& playbackContentValue
    ) :
        audioContentId(audioContentIdValue),
        sharedMetadata(sharedMetadataValue),
        serverContent(serverContentValue),
        playbackContent(playbackContentValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        audioContentId = stream.readString().value_or("");
        sharedMetadata = stream.readString().value_or("");
        serverContent = stream.readString().value_or("");
        playbackContent = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(audioContentId);
		stream.writeString(sharedMetadata);
		stream.writeString(serverContent);
		stream.writeString(playbackContent);
    }
};

} // namespace bedrock::protocol
