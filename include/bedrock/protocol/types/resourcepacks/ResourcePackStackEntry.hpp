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
