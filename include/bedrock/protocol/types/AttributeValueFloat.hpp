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
#include <bedrock/protocol/types/AttributeValue.hpp>

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

    Value value;
    Operation operation;

    AttributeValueFloat() = default;
    virtual ~AttributeValueFloat() = default;
    explicit AttributeValueFloat(const Value& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = magic_enum::enum_cast<AttributeValueFloat::Value>(std::string_view(stream.readFloat().value_or(0.0f))).value_or(AttributeValueFloat::Value::Override);
        operation = magic_enum::enum_cast<AttributeValueFloat::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueFloat::Operation::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeFloat(magic_enum::enum_name(value));
		stream.writeString(magic_enum::enum_name(operation));
    }
};

inline constexpr std::array<std::string_view, 7> AttributeValueFloatOperationNames = {
    "override",
    "alpha_blend",
    "add",
    "subtract",
    "multiply",
    "minimum",
    "maximum",
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<AttributeValueFloat::Operation>(AttributeValueFloat::Operation value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < AttributeValueFloatOperationNames.size() ? customize_t{AttributeValueFloatOperationNames[index]} : invalid_tag;
}

}
