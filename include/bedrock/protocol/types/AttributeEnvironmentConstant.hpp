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
#include <bedrock/protocol/types/AttributeValue.hpp>
#include <bedrock/protocol/types/AttributeEnvironmentPayload.hpp>

class AttributeEnvironmentConstant : public AttributeEnvironmentPayload {
public:
    static constexpr int64_t ID = 0;

    std::shared_ptr<AttributeValue> attribute;

    AttributeEnvironmentConstant() = default;
    virtual ~AttributeEnvironmentConstant() = default;
    explicit AttributeEnvironmentConstant(const std::shared_ptr<AttributeValue>& attributeValue) : attribute(attributeValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        attribute = AttributeValue::decode(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(attribute->getTypeId());
		attribute->write(stream);
    }
};
