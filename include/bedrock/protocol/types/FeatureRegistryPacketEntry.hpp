#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <string>


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
