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

class FeatureRegistryPacketEntry {
public:
    std::string featureName;
    std::string featureJson;

    FeatureRegistryPacketEntry() = default;
    explicit FeatureRegistryPacketEntry(const std::string& featureNameValue, const std::string& featureJsonValue) : featureName(featureNameValue), featureJson(featureJsonValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        featureName = stream.readString().value_or("");
        featureJson = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(featureName);
		stream.writeString(featureJson);
    }
};
