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
#include <bedrock/protocol/types/AttributeValueColorValue.hpp>
#include <bedrock/protocol/types/AttributeValueColorValue.hpp>
#include <bedrock/protocol/types/AttributeValueColorArray.hpp>
#include <bedrock/protocol/types/AttributeValueColorString.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>

class AttributeValueColor : public AttributeValue {
public:
    enum class Operation : uint8_t {
        override = 0,
        alpha_blend = 1,
        add = 2,
        subtract = 3,
        multiply = 4,
    };

    static constexpr int64_t ID = 2;

    std::shared_ptr<AttributeValueColorValue> value;
    Operation operation;

    AttributeValueColor() = default;
    virtual ~AttributeValueColor() = default;
    explicit AttributeValueColor(const std::shared_ptr<AttributeValueColorValue>& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        operation = magic_enum::enum_cast<AttributeValueColor::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueColor::Operation::override);
        value = AttributeValueColorValue::decode(stream);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(value->getTypeId());
		value->write(stream);
		stream.writeString(magic_enum::enum_name(operation));
    }
};
