#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <string>


class ResourcePackInfoEntry {
public:
    std::string packId;
    std::string version;
    uint64_t sizeBytes;
    std::string encryptionKey;
    std::string subPackName;
    std::string contentId;
    bool hasScripts = false;
    bool isAddonPack = false;
    bool isRtxCapable = false;
    std::string cdnUrl;

    ResourcePackInfoEntry() = default;
    explicit ResourcePackInfoEntry(
        const std::string& packIdValue,
        const std::string& versionValue,
        const uint64_t& sizeBytesValue,
        const std::string& encryptionKeyValue,
        const std::string& subPackNameValue,
        const std::string& contentIdValue,
        const bool& hasScriptsValue,
        const bool& isAddonPackValue,
        const bool& isRtxCapableValue,
        const std::string& cdnUrlValue
    ) :
        packId(packIdValue),
        version(versionValue),
        sizeBytes(sizeBytesValue),
        encryptionKey(encryptionKeyValue),
        subPackName(subPackNameValue),
        contentId(contentIdValue),
        hasScripts(hasScriptsValue),
        isAddonPack(isAddonPackValue),
        isRtxCapable(isRtxCapableValue),
        cdnUrl(cdnUrlValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        version = stream.readString().value_or("");
        sizeBytes = stream.readUnsignedLong().value_or(0);
        encryptionKey = stream.readString().value_or("");
        subPackName = stream.readString().value_or("");
        contentId = stream.readString().value_or("");
        hasScripts = stream.readBool().value_or(false);
        isAddonPack = stream.readBool().value_or(false);
        isRtxCapable = stream.readBool().value_or(false);
        cdnUrl = stream.readString().value_or("");
        packId = stream.readUUID().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeUUID(packId);
		stream.writeString(version);
		stream.writeUnsignedLong(sizeBytes);
		stream.writeString(encryptionKey);
		stream.writeString(subPackName);
		stream.writeString(contentId);
		stream.writeBool(hasScripts);
		stream.writeBool(isAddonPack);
		stream.writeBool(isRtxCapable);
		stream.writeString(cdnUrl);
    }
};
