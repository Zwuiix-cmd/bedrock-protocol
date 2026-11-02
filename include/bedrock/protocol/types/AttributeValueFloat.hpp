#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <string_view>

#include <bedrock/protocol/types/AttributeValue.hpp>
namespace bedrock::protocol {


class AttributeValueFloat : public AttributeValue {
public:
    enum class Operation : uint8_t {
        Override = 0,
        AlphaBlend = 1,
        Add = 2,
        Subtract = 3,
        Multiply = 4,
        Minimum = 5,
        Maximum = 6,
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
        const auto operationRaw = stream.readString().value_or("");
        operation = magic_enum::enum_cast<Operation>(
            std::string_view(operationRaw)
        ).value_or(Operation::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(value);
		stream.writeString(magic_enum::enum_name(operation));
    }
};

} // namespace bedrock::protocol

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::AttributeValueFloat::Operation>(::bedrock::protocol::AttributeValueFloat::Operation value) noexcept {
    switch (value) {
        case ::bedrock::protocol::AttributeValueFloat::Operation::Override: return "override";
        case ::bedrock::protocol::AttributeValueFloat::Operation::AlphaBlend: return "alpha_blend";
        case ::bedrock::protocol::AttributeValueFloat::Operation::Add: return "add";
        case ::bedrock::protocol::AttributeValueFloat::Operation::Subtract: return "subtract";
        case ::bedrock::protocol::AttributeValueFloat::Operation::Multiply: return "multiply";
        case ::bedrock::protocol::AttributeValueFloat::Operation::Minimum: return "minimum";
        case ::bedrock::protocol::AttributeValueFloat::Operation::Maximum: return "maximum";
        default: return invalid_tag;
    }
}

}
