#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>

namespace bedrock::protocol {


class ResourcePackStackEntry {
public:
    std::string packId;
    std::string version;
    std::string subPackName;

    ResourcePackStackEntry() = default;
    explicit ResourcePackStackEntry(const std::string& packIdValue, const std::string& versionValue, const std::string& subPackNameValue) : packId(packIdValue), version(versionValue), subPackName(subPackNameValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        packId = stream.readString().value_or("");
        version = stream.readString().value_or("");
        subPackName = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(packId);
		stream.writeString(version);
		stream.writeString(subPackName);
    }
};

} // namespace bedrock::protocol
