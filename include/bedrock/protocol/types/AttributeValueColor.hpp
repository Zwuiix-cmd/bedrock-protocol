#pragma once

#include <bedrock/protocol/io/BinaryStream.hpp>
#include <bedrock/protocol/io/ReadOnlyBinaryStream.hpp>
#include <magic_enum/magic_enum.hpp>
#include <cstdint>
#include <memory>
#include <string_view>

#include <bedrock/protocol/types/AttributeValueColorValue.hpp>
#include <bedrock/protocol/types/AttributeValueColorArray.hpp>
#include <bedrock/protocol/types/AttributeValueColorString.hpp>
#include <bedrock/protocol/types/AttributeValue.hpp>
namespace bedrock::protocol {


class AttributeValueColor : public AttributeValue {
public:
    enum class Operation : uint8_t {
        Override = 0,
        AlphaBlend = 1,
        Add = 2,
        Subtract = 3,
        Multiply = 4,
    };

    static constexpr int64_t ID = 2;

    std::shared_ptr<AttributeValueColorValue> value;
    Operation operation;

    AttributeValueColor() = default;
    virtual ~AttributeValueColor() = default;
    explicit AttributeValueColor(const std::shared_ptr<AttributeValueColorValue>& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = AttributeValueColorValue::create(stream);
        const auto operationRaw = stream.readString().value_or("");
        operation = magic_enum::enum_cast<Operation>(
            std::string_view(operationRaw)
        ).value_or(Operation::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(value->getTypeId());
		value->write(stream);
		stream.writeString(magic_enum::enum_name(operation));
    }
};

} // namespace bedrock::protocol

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<::bedrock::protocol::AttributeValueColor::Operation>(::bedrock::protocol::AttributeValueColor::Operation value) noexcept {
    switch (value) {
        case ::bedrock::protocol::AttributeValueColor::Operation::Override: return "override";
        case ::bedrock::protocol::AttributeValueColor::Operation::AlphaBlend: return "alpha_blend";
        case ::bedrock::protocol::AttributeValueColor::Operation::Add: return "add";
        case ::bedrock::protocol::AttributeValueColor::Operation::Subtract: return "subtract";
        case ::bedrock::protocol::AttributeValueColor::Operation::Multiply: return "multiply";
        default: return invalid_tag;
    }
}

}
