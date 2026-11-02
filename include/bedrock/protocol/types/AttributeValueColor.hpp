#pragma once

#include <array>
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
        Override = 0,
        AlphaBlend = 1,
        Add = 2,
        Subtract = 3,
        Multiply = 4,
    };

    static constexpr int64_t ID = 2;

    Value value;
    Operation operation;

    AttributeValueColor() = default;
    virtual ~AttributeValueColor() = default;
    explicit AttributeValueColor(const Value& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        operation = magic_enum::enum_cast<AttributeValueColor::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueColor::Operation::Override);
        value = magic_enum::enum_cast<AttributeValueColor::Value>(std::string_view(AttributeValueColorValue::decode(stream))).value_or(AttributeValueColor::Value::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeUnsignedVarInt(value->getTypeId());
		value->write(stream);
		stream.writeString(magic_enum::enum_name(operation));
    }
};

inline constexpr std::array<std::string_view, 5> AttributeValueColorOperationNames = {
    "override",
    "alpha_blend",
    "add",
    "subtract",
    "multiply",
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<AttributeValueColor::Operation>(AttributeValueColor::Operation value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < AttributeValueColorOperationNames.size() ? customize_t{AttributeValueColorOperationNames[index]} : invalid_tag;
}

}
