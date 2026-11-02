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

class AttributeValueBool : public AttributeValue {
public:
    enum class Operation : uint8_t {
        override = 0,
        alpha_blend = 1,
        and_ = 2,
        nand = 3,
        or_ = 4,
        nor = 5,
        xor_ = 6,
        xnor = 7,
    };

    static constexpr int64_t ID = 0;

    bool value;
    Operation operation;

    AttributeValueBool() = default;
    virtual ~AttributeValueBool() = default;
    explicit AttributeValueBool(const bool& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readBool().value_or(false);
        operation = magic_enum::enum_cast<AttributeValueBool::Operation>(std::string_view(stream.readString().value_or(""))).value_or(AttributeValueBool::Operation::override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeBool(value);
		stream.writeString(magic_enum::enum_name(operation));
    }
};
