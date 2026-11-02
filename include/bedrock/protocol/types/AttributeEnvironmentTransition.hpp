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
#include <bedrock/protocol/types/AttributeEnvironmentTransitionSettings.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentTransitionSettings.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentTransitionSettings.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>

class AttributeEnvironmentTransition : public AttributeEnvironmentPayload {
public:
    static constexpr int64_t ID = 1;

    std::shared_ptr<AttributeValue> fromAttribute;
    std::shared_ptr<AttributeValue> toAttribute;
    std::shared_ptr<AttributeEnvironmentTransitionSettings> settings;

    AttributeEnvironmentTransition() = default;
    virtual ~AttributeEnvironmentTransition() = default;
    explicit AttributeEnvironmentTransition(
        const std::shared_ptr<AttributeValue>& fromAttributeValue,
        const std::shared_ptr<AttributeValue>& toAttributeValue,
        const std::shared_ptr<AttributeEnvironmentTransitionSettings>& settingsValue
    ) :
        fromAttribute(fromAttributeValue),
        toAttribute(toAttributeValue),
        settings(settingsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        fromAttribute = AttributeValue::decode(stream);
        toAttribute = AttributeValue::decode(stream);
        settings = ([&]() {
    auto value = std::make_shared<AttributeEnvironmentTransitionSettings>();
    value->read(stream);
    return value;
}());
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(fromAttribute->getTypeId());
		fromAttribute->write(stream);
		stream.writeUnsignedVarInt(toAttribute->getTypeId());
		toAttribute->write(stream);
		settings->write(stream);
    }
};
