#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/AttributeEnvironmentNoiseTransitionSettings.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>
namespace bedrock::protocol {


class AttributeEnvironmentNoiseTransition : public AttributeEnvironmentPayload {
public:
    static constexpr int64_t ID = 2;

    std::shared_ptr<AttributeValue> fromAttribute;
    std::shared_ptr<AttributeValue> toAttribute;
    std::shared_ptr<AttributeEnvironmentNoiseTransitionSettings> settings;

    AttributeEnvironmentNoiseTransition() = default;
    virtual ~AttributeEnvironmentNoiseTransition() = default;
    explicit AttributeEnvironmentNoiseTransition(
        const std::shared_ptr<AttributeValue>& fromAttributeValue,
        const std::shared_ptr<AttributeValue>& toAttributeValue,
        const std::shared_ptr<AttributeEnvironmentNoiseTransitionSettings>& settingsValue
    ) :
        fromAttribute(fromAttributeValue),
        toAttribute(toAttributeValue),
        settings(settingsValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        fromAttribute = AttributeValue::create(stream);
        toAttribute = AttributeValue::create(stream);
        settings = std::make_shared<AttributeEnvironmentNoiseTransitionSettings>();
        settings->read(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(fromAttribute->getTypeId());
		fromAttribute->write(stream);
		stream.writeUnsignedVarInt(toAttribute->getTypeId());
		toAttribute->write(stream);
		settings->write(stream);
    }
};

} // namespace bedrock::protocol
