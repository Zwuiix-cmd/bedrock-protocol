#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstddef>
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

    static std::shared_ptr<AttributeLayer> create(ReadOnlyBinaryStream& stream) {
		auto name = stream.readString().value_or("");
		auto dimension = stream.readUnsignedVarInt().value_or(0);
		auto settings = std::make_shared<AttributeLayerSettings>();
		settings->read(stream);
		auto attributes = std::vector<std::shared_ptr<AttributeEnvironment>>();
		auto attributesCount = stream.readUnsignedVarInt().value_or(0);
		const bool attributesCountAvailable = stream.canReadCollection(attributesCount);
		for (std::size_t i = 0; i < attributesCount && attributesCountAvailable && !stream.hasOverflowed(); ++i) {
		    attributes.push_back(stream.readObject<AttributeEnvironment>());
		}
		return std::make_shared<AttributeLayer>(name, static_cast<int32_t>(dimension), settings, attributes);
    }

    virtual void read(ReadOnlyBinaryStream& stream) {
        auto decoded = create(stream);
        if (decoded) {
            *this = *decoded;
        }
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
