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

class AttributeValueFloat : public AttributeValue {
public:
    enum class Operation : uint8_t {
        override = 0,
        alpha_blend = 1,
        add = 2,
        subtract = 3,
        multiply = 4,
        minimum = 5,
        maximum = 6,
    };

    static constexpr int64_t ID = 1;

    float value;
    Operation operation;

    AttributeValueFloat() = default;
    virtual ~AttributeValueFloat() = default;
    explicit AttributeValueFloat(const float& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readFloat().value_or(0.0f);
        operation = magic_enum::enum_cast<AttributeValueFloat::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueFloat::Operation::override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(value);
		stream.writeString(magic_enum::enum_name(operation));
    }
};
