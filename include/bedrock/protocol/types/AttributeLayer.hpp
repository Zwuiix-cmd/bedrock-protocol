#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

#include <bedrock/protocol/types/AttributeLayerSettings.hpp>
#include <bedrock/protocol/types/AttributeEnvironment.hpp>

class AttributeLayer {
public:
    std::string name;
    uint32_t dimension;
    std::shared_ptr<AttributeLayerSettings> settings;
    std::vector<std::shared_ptr<AttributeEnvironment>> attributes;

    AttributeLayer() = default;
    explicit AttributeLayer(
        const std::string& nameValue,
        const uint32_t& dimensionValue,
        const std::shared_ptr<AttributeLayerSettings>& settingsValue,
        const std::vector<std::shared_ptr<AttributeEnvironment>>& attributesValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        settings(settingsValue),
        attributes(attributesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        dimension = stream.readUnsignedVarInt().value_or(0);
        settings = std::make_shared<AttributeLayerSettings>();
        settings->read(stream);
        attributes = stream.readObjectList<AttributeEnvironment>();
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeUnsignedVarInt(dimension);
		settings->write(stream);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			attribute->write(stream);
		}
    }
};
