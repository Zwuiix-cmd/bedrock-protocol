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
#include <bedrock/protocol/types/AttributeLayerSettings.hpp>
#include <bedrock/protocol/types/AttributeEnvironment.hpp>
#include <bedrock/protocol/types/AttributeLayerSettings.hpp>
#include <bedrock/protocol/types/AttributeEnvironment.hpp>
#include <bedrock/protocol/types/AttributeLayerSettings.hpp>

class AttributeLayer {
public:
    std::string name;
    int32_t dimension;
    std::shared_ptr<AttributeLayerSettings> settings;
    std::vector<std::shared_ptr<AttributeEnvironment>> attributes;

    AttributeLayer() = default;
    explicit AttributeLayer(
        const std::string& nameValue,
        const int32_t& dimensionValue,
        const std::shared_ptr<AttributeLayerSettings>& settingsValue,
        const std::vector<std::shared_ptr<AttributeEnvironment>>& attributesValue
    ) :
        name(nameValue),
        dimension(dimensionValue),
        settings(settingsValue),
        attributes(attributesValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        dimension = stream.readVarInt().value_or(0);
        settings = ([&]() {
    auto value = std::make_shared<AttributeLayerSettings>();
    value->read(stream);
    return value;
}());
        attributes = stream.readList([&]() {
            return ([&]() {
            auto value = std::make_shared<AttributeEnvironment>();
            value->read(stream);
            return value;
            }());
        });
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeVarInt(dimension);
		settings->write(stream);
		stream.writeUnsignedVarInt(attributes.size());
		for (const auto& attribute : attributes) {
			attribute->write(stream);
		}
    }
};
