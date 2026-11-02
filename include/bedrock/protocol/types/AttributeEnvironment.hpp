#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>

#include <bedrock/protocol/types/AttributeValue.hpp>

class AttributeEnvironment {
public:
    std::string name;
    std::optional<std::shared_ptr<AttributeValue>> fromAttribute;
    std::shared_ptr<AttributeValue> attribute;
    std::optional<std::shared_ptr<AttributeValue>> toAttribute;
    uint32_t currentTransitionTicks;
    uint32_t totalTransitionTicks;
    std::string easeType;

    AttributeEnvironment() = default;
    explicit AttributeEnvironment(
        const std::string& nameValue,
        const std::optional<std::shared_ptr<AttributeValue>>& fromAttributeValue,
        const std::shared_ptr<AttributeValue>& attributeValue,
        const std::optional<std::shared_ptr<AttributeValue>>& toAttributeValue,
        const uint32_t& currentTransitionTicksValue,
        const uint32_t& totalTransitionTicksValue,
        const std::string& easeTypeValue
    ) :
        name(nameValue),
        fromAttribute(fromAttributeValue),
        attribute(attributeValue),
        toAttribute(toAttributeValue),
        currentTransitionTicks(currentTransitionTicksValue),
        totalTransitionTicks(totalTransitionTicksValue),
        easeType(easeTypeValue) {}

    virtual void read(ReadOnlyBinaryStream& stream) {
        name = stream.readString().value_or("");
        fromAttribute = stream.readOptional([&]() {
            return AttributeValue::create(stream);
        });
        attribute = AttributeValue::create(stream);
        toAttribute = stream.readOptional([&]() {
            return AttributeValue::create(stream);
        });
        currentTransitionTicks = stream.readUnsignedInt().value_or(0);
        totalTransitionTicks = stream.readUnsignedInt().value_or(0);
        easeType = stream.readString().value_or("");
    }

    virtual void write(BinaryStream& stream) const {
		stream.writeString(name);
		stream.writeOptional(fromAttribute, [&](const auto& value) {
			value->write(stream);
		});
		attribute->write(stream);
		stream.writeOptional(toAttribute, [&](const auto& value) {
			value->write(stream);
		});
		stream.writeUnsignedInt(currentTransitionTicks);
		stream.writeUnsignedInt(totalTransitionTicks);
		stream.writeString(easeType);
    }
};
