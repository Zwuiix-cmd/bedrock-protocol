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

    bool value;
    Operation operation;

    AttributeValueBool() = default;
    virtual ~AttributeValueBool() = default;
    explicit AttributeValueBool(const bool& valueValue, const Operation& operationValue) : value(valueValue), operation(operationValue) {}

    int32_t getTypeId() const override { return static_cast<int32_t>(ID); }

    void read(ReadOnlyBinaryStream& stream) override {
        value = stream.readBool().value_or(false);
        const auto operationRaw = stream.readString().value_or("");
        operation = magic_enum::enum_cast<Operation>(
            std::string_view(operationRaw)
        ).value_or(Operation::Override);
    }

    void write(BinaryStream& stream) const override {
		stream.writeBool(magic_enum::enum_name(value));
		stream.writeString(magic_enum::enum_name(operation));
    }
};

namespace magic_enum::customize {

template <> constexpr customize_t enum_name<AttributeValueBool::Operation>(AttributeValueBool::Operation value) noexcept {
    switch (value) {
        case AttributeValueBool::Operation::Override: return "override";
        case AttributeValueBool::Operation::AlphaBlend: return "alpha_blend";
        case AttributeValueBool::Operation::And: return "and";
        case AttributeValueBool::Operation::Nand: return "nand";
        case AttributeValueBool::Operation::Or: return "or";
        case AttributeValueBool::Operation::Nor: return "nor";
        case AttributeValueBool::Operation::Xor: return "xor";
        case AttributeValueBool::Operation::Xnor: return "xnor";
        default: return invalid_tag;
    }
}

}
