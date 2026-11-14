#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <cstdint>
#include <memory>

#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>
namespace bedrock::protocol {


class AttributeEnvironmentConstant : public AttributeEnvironmentPayload {
public:
    static constexpr int64_t ID = 0;

    std::shared_ptr<AttributeValue> attribute;

    AttributeEnvironmentConstant() = default;
    virtual ~AttributeEnvironmentConstant() = default;
    explicit AttributeEnvironmentConstant(const std::shared_ptr<AttributeValue>& attributeValue) : attribute(attributeValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        attribute = AttributeValue::create(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(attribute->getTypeId());
		attribute->write(stream);
    }
};

} // namespace bedrock::protocol
