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

class AttributeValueBool : public AttributeValue {
public:
    enum class Operation : uint8_t {
        Override = 0,
        AlphaBlend = 1,
        And = 2,
        Nand = 3,
        Or = 4,
        Nor = 5,
        Xor = 6,
        Xnor = 7,
    };

    static constexpr int64_t ID = 0;

    Value value;
    Operation operation;

    AttributeValueBool() = default;
    virtual ~AttributeValueBool() = default;
    explicit AttributeValueBool(const Value& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = magic_enum::enum_cast<AttributeValueBool::Value>(std::string_view(stream.readBool().value_or(false))).value_or(AttributeValueBool::Value::Override);
        operation = magic_enum::enum_cast<AttributeValueBool::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueBool::Operation::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeBool(magic_enum::enum_name(value));
		stream.writeString(magic_enum::enum_name(operation));
    }
};

inline constexpr std::array<std::string_view, 8> AttributeValueBoolOperationNames = {
    "override",
    "alpha_blend",
    "and",
    "nand",
    "or",
    "nor",
    "xor",
    "xnor",
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<AttributeValueBool::Operation>(AttributeValueBool::Operation value) noexcept {
    const auto index = static_cast<std::size_t>(::magic_enum::enum_integer(value));
    return index < AttributeValueBoolOperationNames.size() ? customize_t{AttributeValueBoolOperationNames[index]} : invalid_tag;
}

}
